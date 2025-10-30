#!/usr/bin/env bash

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

# Fancy output
C_RED='\033[1;31m'
C_PURPLE='\033[1;35m'
C_GREEN='\033[1;32m'
C_RESET='\033[0m'

# Print error
error() {
  echo -e "${C_RED}Error:${C_RESET} $1"
}

# Print heading
heading() {
  echo -e "${C_PURPLE}$1${C_RESET}"
}

# Print ok
ok() {
  echo -e "${C_GREEN}Done${C_RESET}"
}

# Die if error
possibly_die() {
  if [ $? = 1 ]; then
    error $1
    exit 1
  fi
}

# Branches
BRANCHES="dev-ak dev-jm dev-jr dev-mb dev-mm dev-mp"

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
  ok
elif [ "$MODE" = "merge" ]; then   # merge
  heading "# Merging to main"
  git switch main
  for branch in ${BRANCHES}; do
    git merge ${branch}
    possibly_die "Failed to merge ${branch}"
  done
  git switch main
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
  heading "# Pushing rebased branches"
  for branch in ${BRANCHES}; do
    git switch ${branch}
    possibly_die "Failed to switch to ${branch}"
    git push
    possibly_die "Failed to push ${branch}"
  done
  ok
  # Push main
  git switch main
  possibly_die "Failed to switch to main"
  git push
  possibly_die "Failed to push main"
  # Switch back home
  git switch dev-mm
  possibly_die "Failed to switch to dev-mm"
else # invalid argument
  help_and_die
fi
