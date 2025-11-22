# CI/CD Workflows

This directory contains GitHub Actions workflows for continuous integration and deployment.

## Workflow Status

If workflows are failing, check:

1. **Build Errors**: The code may have compilation errors. Check the build logs.
2. **Missing Dependencies**: Ensure all dependencies are installed in the workflow.
3. **Script Errors**: Verification scripts may need adjustments.
4. **Configuration Issues**: CMake configuration may need updates.

## Workflow Resilience

Most workflows use `continue-on-error: true` for non-critical steps to:
- Report errors without failing the entire workflow
- Allow partial success
- Provide better debugging information

## Fixing Workflow Failures

1. Check the workflow logs for specific error messages
2. Test locally using the same commands
3. Fix the underlying issue (compilation errors, missing files, etc.)
4. Re-run the workflow

## Disabling Workflows

If a workflow is causing issues, you can temporarily disable it by:
- Commenting out the workflow file
- Or adding a condition that prevents it from running

