/* jsmn.c - Minimal jsmn implementation (public domain) */
#include "jsmn.h"
#include <string.h>
#include <stdlib.h>

/* Implementation adapted from jsmn 1.2 */

static int jsmn_alloc_token(jsmn_parser *parser, jsmntok_t *tokens, size_t num_tokens) {
    if (parser->toknext >= num_tokens) return -1;
    jsmntok_t *tok = &tokens[parser->toknext++];
    tok->start = tok->end = -1;
    tok->size = 0;
#ifdef JSMN_PARENT_LINKS
    tok->parent = -1;
#endif
    return parser->toknext - 1;
}

void jsmn_init(jsmn_parser *parser) {
    parser->pos = 0;
    parser->toknext = 0;
    parser->toksuper = -1;
}

static int jsmn_parse_primitive(jsmn_parser *parser, const char *js, size_t len,
                                 jsmntok_t *tokens, size_t num_tokens) {
    int start = parser->pos;
    while (parser->pos < len) {
        char c = js[parser->pos];
        if (c == '\t' || c == '\r' || c == '\n' || c == ' ' || c == ',' || c == ']' || c == '}') {
            break;
        }
        parser->pos++;
    }
    int idx = jsmn_alloc_token(parser, tokens, num_tokens);
    if (idx == -1) return -1;
    tokens[idx].type = JSMN_PRIMITIVE;
    tokens[idx].start = start;
    tokens[idx].end = parser->pos;
    tokens[idx].size = 0;
#ifdef JSMN_PARENT_LINKS
    tokens[idx].parent = parser->toksuper;
#endif
    parser->pos--;
    return 0;
}

static int jsmn_parse_string(jsmn_parser *parser, const char *js, size_t len,
                             jsmntok_t *tokens, size_t num_tokens) {
    int start = parser->pos++;
    while (parser->pos < len) {
        char c = js[parser->pos];
        if (c == '"') {
            int idx = jsmn_alloc_token(parser, tokens, num_tokens);
            if (idx == -1) return -1;
            tokens[idx].type = JSMN_STRING;
            tokens[idx].start = start + 1;
            tokens[idx].end = parser->pos;
            tokens[idx].size = 0;
#ifdef JSMN_PARENT_LINKS
            tokens[idx].parent = parser->toksuper;
#endif
            return 0;
        }
        if (c == '\\') parser->pos++;
        parser->pos++;
    }
    return -1;
}

int jsmn_parse(jsmn_parser *parser, const char *js, size_t len,
               jsmntok_t *tokens, unsigned int num_tokens) {
    int r;
    for (; parser->pos < len; parser->pos++) {
        char c = js[parser->pos];
        switch (c) {
            case '{':
            case '[': {
                int idx = jsmn_alloc_token(parser, tokens, num_tokens);
                if (idx == -1) return -1;
                tokens[idx].type = (c == '{') ? JSMN_OBJECT : JSMN_ARRAY;
                tokens[idx].start = parser->pos;
#ifdef JSMN_PARENT_LINKS
                tokens[idx].parent = parser->toksuper;
                if (parser->toksuper != -1) tokens[parser->toksuper].size++;
                parser->toksuper = idx;
#else
                parser->toksuper = idx;
#endif
                break;
            }
            case '}':
            case ']': {
                jsmntype_t type = (c == '}') ? JSMN_OBJECT : JSMN_ARRAY;
                int i;
                for (i = parser->toknext - 1; i >= 0; i--) {
#ifdef JSMN_PARENT_LINKS
                    if (tokens[i].start != -1 && tokens[i].end == -1) {
                        if (tokens[i].type == type) {
                            tokens[i].end = parser->pos + 1;
                            parser->toksuper = tokens[i].parent;
                            break;
                        }
                    }
#else
                    if (tokens[i].start != -1 && tokens[i].end == -1) {
                        if (tokens[i].type == type) {
                            tokens[i].end = parser->pos + 1;
                            parser->toksuper = -1;
                            break;
                        }
                    }
#endif
                }
                break;
            }
            case '"':
                r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
                if (r != 0) return -1;
                break;
            case '\t':
            case '\r':
            case '\n':
            case ' ':
            case ':':
            case ',':
                break;
            default:
                r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
                if (r != 0) return -1;
                break;
        }
    }
    return parser->toknext;
}
