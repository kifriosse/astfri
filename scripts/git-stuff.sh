#!/usr/bin/env bash

# Include io utils.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/fancy-io.sh"

# Print help and die
help_and_die() {
  echo "Provide single argument. One of:"
  echo "  pull"
  echo "  merge"
  echo "  rebase"
  echo "  push"
  exit 1
}

# Check argument count
if [ "$#" -ne 1 ]; then
  help_and_die
fi

# Script mode
MODE=$1

# Branches
BRANCHES="dev-ak dev-jm dev-jr dev-mb dev-mm dev-mp dev-ab dev-jk"

if [ "$MODE" = "pull" ]; then    # pull
  heading "# Pulling changes"
  git switch main
  git pull
  for branch in ${BRANCHES}; do
    git switch ${branch}
    possibly_die "Failed to switch to ${branch}"
    git pull
    possibly_die "Failed to pull ${branch}"
  done
  git switch main
  possibly_die "Failed to switch to main"
  ok
elif [ "$MODE" = "merge" ]; then   # merge
  heading "# Merging to main"
  git switch main
  for branch in ${BRANCHES}; do
    git merge ${branch}
    possibly_die "Failed to merge ${branch}"
  done
  # git switch main
  ok
elif [ "$MODE" = "rebase" ]; then  # rebase
  heading "# Rebasing on main"
  for branch in ${BRANCHES}; do
    git switch ${branch}
    possibly_die "Failed to switch to ${branch}"
    git rebase main
    possibly_die "Failed to rebase ${branch}"
  done
  git switch main
  possibly_die "Failed to switch to main"
  ok
elif [ "$MODE" = "push" ]; then    # push
  heading "# Pushing all branches"
  git push --all
  git push --tags
  # Switch back home
  git switch dev-mm
  possibly_die "Failed to switch to dev-mm"
else # invalid argument
  help_and_die
fi
