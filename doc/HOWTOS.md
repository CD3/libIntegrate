This is a collection of short how-tos for various tasks that are often needed but easy to forget.

# `git subrepo`

## Cloning a new external

Say you have an external repository named MyProject that is located at git@gitlab.com:rtfm/MyProject. Do add
this repository to your project as an extenral, run.

    > git subrepo clone git@gitlab.com:rtfm/MyProject externals/MyProject

## Pushing changes upstream

`git subrepo` can push changes you make to the extneral repository back up stream. To avoid errors, you should
always commit edits to each external repo directory sepratly. In other words, don't commit a commit that contains changes
in your project *and* the extenral project, or changes in more than one external project at a time.
To push changes back upstream to MyProject, you need to first make sure that you have the latest commits from upstream, then
push your commits.

    > git subrepo pull externals/MyProject # will try to rebase your changes against the upstream HEAD. could fail if you have conflicting changes.
    > git subrepo push externals/MyProject # will push your commits upstream
    > git subrepo pull externals/MyProject # may not be necessary, but just incase a new commit is created and pushed, update your repo so it matches upstream.

