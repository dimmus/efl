---
title: Contributing
category: Contributing
layout: default
SPDX-License-Identifier: LGPL-2.1-or-later
---

# Contributing

We welcome contributions from everyone. However, please follow the following guidelines when posting a GitHub Pull Request or filing a GitHub Issue on the systemd project:

## Quick start

 1. Fork the repository
 2. Create a branch (`git checkout -b my_work`)
 3. Commit your changes (`git commit -am "Added my awesome feature"`)
 4. Push to the branch (`git push origin my_work`)
 5. Open a new merge request
 6. Sit back, relax and wait for feedback and eventual merge

## Commit Messages

- Always add a brief description of the commit to the **first** line of
  the commit and terminate by two newlines (it will work without the
  second newline, but that is not nice for the interfaces).

    short description          - MUST be less than 72 characters
    <newline>                  - MANDATORY empty line
    long description           - Each line MUST be less than 76 characters

- The first line should be limited to 50 characters and should be a sentence that
  completes the thought [When applied, this commit will...] *"Implement
  cmd_move"* or *"Fix #742"* or *"Improve performance of arrange_windows on ARM"*
  or similar.

- The subsequent lines should be separated from the subject line by a single
  blank line, and include optional details. In this you can give justification
  for the change, [reference Github
  issues](https://help.github.com/articles/closing-issues-via-commit-messages/),
  or explain some of the subtler details of your patch. This is important because
  when someone finds a line of code they don't understand later, they can use the
  `git blame` command to find out what the author was thinking when they wrote
  it. It's also easier to review your pull requests if they're separated into
  logical commits that have good commit messages and justify themselves in the
  extended commit description.

  As a good rule of thumb, anything you might put into the pull request
  description on Github is probably fair game for going into the extended commit
  message as well.

  See [here](https://chris.beams.io/posts/git-commit/) for more details.

- Do NOT put the commit message on the short description line. One line
  commit messages should be avoided, unless they can be **fully** explained
  in less than 72 characters (e.g. "Fix typo in 
  `clutter_actor_create_pango_context()` docs").

- The brief description might optionally have a "tag", i.e. a word or two
  followed by a color, detailing what part of the repository the commit
  affected, e.g.: `alpha: Add :mode property`

- The tag counts as part of overall character count, so try using
  a short word. Optionally, you can also use the `[tag]` form.

- Build environment fixes should use the `build` tag.

- Think of the commit message as an email sent to the maintainers explaining
  "what" you did and, more importantly, "why" you did it. The "how" is not
  important, since "git show" will show the patch inlined with the commit
  message.

## Filing Issues

* We use [GitHub Issues](https://github.com/systemd/systemd/issues) **exclusively** for tracking **bugs** and **feature** **requests** (RFEs) of systemd.
  If you are looking for help, please try the forums of your distribution first, or [systemd-devel mailing list](https://lists.freedesktop.org/mailman/listinfo/systemd-devel) for general questions about systemd.
* We only track bugs in the **two** **most** **recently** **released** (non-rc) **versions** of systemd in the GitHub Issue tracker.
  If you are using an older version of systemd, please contact your distribution's bug tracker instead (see below).
  See [GitHub Release Page](https://github.com/systemd/systemd/releases) for the list of most recent releases.
* When filing a feature request issue (RFE), please always check first if the newest upstream version of systemd already implements the feature,
  and whether there's already an issue filed for your feature by someone else.
* When filing an issue, specify the **systemd** **version** you are experiencing the issue with. Also, indicate which **distribution** you are using.
* Please include an explanation how to reproduce the issue you are pointing out.

Following these guidelines makes it easier for us to process your issue, and ensures we won't close your issue right-away for being misfiled.

## Posting Pull Requests

If you already have your own pull request habits, feel free to use them. If you
don't, however, allow me to make a suggestion: feature branches pulled from
upstream. Try this:

1. Fork EFL
2. `git clone https://github.com/dimmus/efl && cd efl`
3. `git remote add upstream https://github.com/dimmus/efl`

You only need to do this once. You're never going to use your fork's master
branch. Instead, when you start working on a feature, do this:

1. `git fetch upstream`
2. `git checkout -b add-so-and-so-feature upstream/master`
3. Add and commit your changes
4. `git push -u origin add-so-and-so-feature`
5. Make a pull request from your feature branch

When you submit your pull request, your commit log should do most of the talking
when it comes to describing your changes and their motivation. In addition to
this, your pull request's comments will ideally include a test plan that the
reviewers can use to (1) demonstrate the problem on master, if applicable and
(2) verify that the problem no longer exists with your changes applied (or that
your new features work correctly). Document all of the edge cases you're aware
of so we can adequately test them - then verify the test plan yourself before
submitting.

* Make sure to post PRs only relative to a recent tip of the `main` branch.
* Follow our [Coding Style](/CODING_STYLE) when contributing code. This is a requirement for all code we merge.
* Please make sure to test your change before submitting the PR. See the [Hacking guide](/HACKING) for details on how to do this.
* Make sure to run the test suite locally, before posting your PR. We use a CI system, meaning we don't even look at your PR if the build and tests don't pass.
* If you need to update the code in an existing PR, force-push into the same branch, overriding old commits with new versions.
* After you have pushed a new version, add a comment explaining the latest changes.
  If you are a member of the systemd project on GitHub, remove the `reviewed/needs-rework`/`ci-fails/needs-rework`/`needs-rebase` labels.
* If you are copying existing code from another source (eg: a compat header), please make sure the license is compatible with `LGPL-2.1-or-later`.
  If the license is not `LGPL-2.1-or-later`, please add a note to [`LICENSES/README.md`](https://github.com/systemd/systemd/blob/main/LICENSES/README.md).
* If the pull request stalls without review, post a ping in a comment after some time has passed.
  We are always short on reviewer time, and pull requests which haven't seen any recent activity can be easily forgotten.
* Github will automatically add the `please-review` label when a pull request is opened or updated.
  If you need more information after a review, you can comment `/please-review` on the pull request to have Github add the `please-review` label to the pull request.

## Reviewing Pull Requests

All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.All patches, even trivial ones, require at least one positive review
(Reviewed-by). Additionally, if no Reviewed-by's have been given by
people with commit access, there needs to be at least one Acked-by from
someone with commit access. A person with commit access is expected to be
able to evaluate the patch with respect to the project scope and architecture.

The below review guidelines are intended to be interpreted in spirit, not by
the letter. There may be circumstances where some guidelines are better
ignored. We rely very much on the judgement of reviewers and commit rights
holders.

During review, the following matters should be checked:

- The commit message explains why the change is being made.

- The code fits the project's scope.

- The code license is the same MIT licence the project generally uses.

- Stable ABI or API is not broken.

- Stable ABI or API additions must be justified by actual use cases, not only
by speculation. They must also be documented, and it is strongly recommended to
include tests exercising the additions in the test suite.

- The code fits the existing software architecture, e.g. no layering
violations.

- The code is correct and does not introduce new failures for existing users,
does not add new corner-case bugs, and does not introduce new compiler
warnings.

- The patch does what it says in the commit message and changes nothing else.

- The patch is a single logical change. If the commit message addresses
multiple points, it is a hint that the commit might need splitting up.

- A bug fix should target the underlying root cause instead of hiding symptoms.
If a complete fix is not practical, partial fixes are acceptable if they come
with code comments and filed Gitlab issues for the remaining bugs.

- The bug root cause rule applies to external software components as well, e.g.
do not work around kernel driver issues in userspace.

- The test suite passes.

- The code does not depend on API or ABI which has no working free open source
implementation.

- The code is not dead or untestable. E.g. if there are no free open source
software users for it then it is effectively dead code.

- The code is written to be easy to understand, or if code cannot be clear
enough on its own there are code comments to explain it.

- The code is minimal, i.e. prefer refactor and re-use when possible unless
clarity suffers.

- The code adheres to the style guidelines.

- In a patch series, every intermediate step adheres to the above guidelines.

* See [filtered list of pull requests](https://github.com/systemd/systemd/pulls?q=is%3Aopen+is%3Apr+-label%3A%22reviewed%2Fneeds-rework+%F0%9F%94%A8%22+-label%3Aneeds-rebase+-label%3Agood-to-merge%2Fwith-minor-suggestions+-label%3A%22good-to-merge%2Fwaiting-for-ci+%F0%9F%91%8D%22+-label%3Apostponed+-label%3A%22needs-reporter-feedback+%E2%9D%93%22+-label%3A%22dont-merge+%F0%9F%92%A3%22+-label%3A%22ci-fails%2Fneeds-rework+%F0%9F%94%A5%22+sort%3Aupdated-desc) for requests that are ready for review.
* After performing a review, set

  * `reviewed/needs-rework` if the pull request needs significant changes
  * `ci-fails/needs-rework` if the automatic tests fail and the failure is relevant to the pull request
  * `ci-failure-appears-unrelated` if the test failures seem irrelevant
  * `needs-rebase` if the pull request needs a rebase because of conflicts
  * `good-to-merge/waiting-for-ci` if the pull request should be merged without further review
  * `good-to-merge/with-minor-suggestions` if the pull request should be merged after an update without going through another round of reviews

Unfortunately only members of the `efl` organization on github can change labels.
If your pull request is mislabeled, make a comment in the pull request and somebody will fix it.
Reviews from non-members are still welcome.

## Final Words

We'd like to apologize in advance if we are not able to process and reply to your issue or PR right-away. We have a lot of work to do, but we are trying our best!

Thank you very much for your contributions!

# Backward Compatibility And External Dependencies

We strive to keep backward compatibility where possible and reasonable.
The following are general guidelines, not hard rules, and case-by-case exceptions might be applied at the discretion of the maintainers.
The current set of build-time and runtime dependencies are documented in the [REQUIREMENTS](https://github.com/dimmus/efl/main/doc/manuals/REQUIREMENTS.md).

## New features

It is fine for new features/functionality/tools/daemons to require bleeding edge external dependencies, provided there
are runtime and build-time graceful fallbacks (e.g.: a daemon will not be built, runtime functionality will be skipped with a clear log message).
In case a new feature is added to both `efl` and one of its dependencies, we expect the corresponding feature code to
be merged upstream in the dependency before accepting our side of the implementation.

## External Build/Runtime Dependencies

It is often tempting to bump external dependencies' minimum versions to cut cruft, and in general it's an essential part
of the maintenance process. But as a general rule, existing dependencies should not be bumped without strong
reasons. When possible, we try to keep compatibility with the most recent LTS releases of each mainstream distribution
for optional components, and with all currently maintained (i.e.: not EOL) LTS releases for core components. When in
doubt, ask before committing time to work on contributions if it's not clear that cutting support would be obviously
acceptable.

# Commit Rights

Commit rights will be granted to anyone who requests them and fulfills the
below criteria:

- Submitted a few (5-10 as a rule of thumb) non-trivial (not just simple
  spelling fixes and whitespace adjustment) patches that have been merged
  already.

- Are actively participating on discussions about their work (on the mailing
  list or IRC). This should not be interpreted as a requirement to review other
  peoples patches but just make sure that patch submission isn't one-way
  communication. Cross-review is still highly encouraged.

- Will be regularly contributing further patches.

- Agrees to use their commit rights in accordance with the documented merge
  criteria, tools, and processes.

To apply for commit rights ("Developer" role) send a mail to
enlightenment-devel@lists.freedesktop.org and please ping the maintainers if your request
is stuck.

Committers are encouraged to request their commit rights get removed when they
no longer contribute to the project. Commit rights will be reinstated when they
come back to the project.

Maintainers and committers should encourage contributors to request commit
rights, as especially junior contributors tend to underestimate their skills.

# Stabilising for releases

A release cycle ends with a stable release which also starts a new cycle and
lifts any code freezes. Gradual code freezing towards a stable release starts
with an alpha release. The release stages of a cycle are:

- **Alpha release**:
    Signified by version number #.#.91.
    Major features must have landed before this. Major features include
    invasive code motion and refactoring, high risk changes, and new stable
    library ABI.

- **Beta release**:
    Signified by version number #.#.92.
    Minor features must have landed before this. Minor features include all
    new features that are not major, low risk changes, clean-ups, and
    documentation. Stable ABI that was new in the alpha release can be removed
    before a beta release if necessary.

- **Release candidates (RC)**:
    Signified by version number #.#.93 and up to #.#.99.
    Bug fixes that are not release critical must have landed before this.
    Release critical bug fixes can still be landed after this, but they may
    call for another RC.

- **Stable release**:
    Signified by version number #.#.0.
    Ideally no changes since the last RC.

Mind that version #.#.90 is never released. It is used during development when
no code freeze is in effect. Stable branches and point releases are not covered
by the above.
