# Releases and tagging (alpha)

This repository uses an automated alpha release workflow. Best-practice defaults are installed on the branch `copilot/fix-address-space-type-issue`.

How tagging works (defaults)
- Tag format: `alpha-ISSUE-<number>-<short-slug>` (example: `alpha-ISSUE-123-fix-npdm`).
- Triggers:
  - Add the label `release` to an issue -> the `Create Story Tag` workflow (when merged to the default branch) will create a tag for that issue.
  - Manually run the `Create Story Tag` workflow and provide an `issue_number` and optional `branch` to create a tag.
  - Pushing a tag that matches `alpha-*` will trigger the `Alpha Release` workflow which builds artifacts and creates a prerelease with separate `.nro` and `.nsp` assets (if produced by the build).

Notes and best-practices
- Workflows that respond to repository events (issues.labeled) must be present in the repository default branch (usually `main`) to trigger automatically. Create a PR to merge the workflow files into `main` to enable automatic tagging on label events.
- The `Alpha Release` workflow builds using the devkitPro toolchain. Producing a signed `.nsp` may require secrets and signing keys; unsigned NSPs may not be installable on retail hardware.
- If your Makefile outputs artifacts to paths other than `build/awoo-installer.nro` or the repo root, update `.github/workflows/alpha-release.yml` to match the actual output paths.

How to trigger a tag manually (local):

```bash
# ensure you are on the branch you want to tag
git fetch origin
git checkout copilot/fix-address-space-type-issue
git pull origin copilot/fix-address-space-type-issue

# create annotated tag and push
git tag -a alpha-20251218 -m "alpha tag 2025-12-18"
git push origin refs/tags/alpha-20251218
```
