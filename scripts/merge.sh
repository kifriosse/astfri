#!/bin/bash

# Fancy output
C_RED='\033[1;31m'
C_PURPLE='\033[1;35m'
C_GREEN='\033[1;32m'
C_RESET='\033[0m'

error() {
  echo -e "${C_RED}Error:${C_RESET} $1"
}

heading() {
  echo -e "${C_PURPLE}$1${C_RESET}"
}

ok() {
  echo -e "${C_GREEN}Done${C_RESET}"
}

possibly_die() {
  if [ $? -eq 1 ]; then
    error $1
    exit 1
  fi
}

# Branches
BRANCHES="dev-ak dev-jm dev-jr dev-mb dev-mm dev-mp"

# Pull newest changes in each branch
heading "# Pulling changes"
for branch in ${BRANCHES}; do
  git switch ${branch}
  possibly_die "Failed to switch to ${branch}"

  git pull
  possibly_die "Failed to pull ${branch}"
done
ok

# Merge each branch to main
heading "# Merging to main"
git switch main
for branch in ${BRANCHES}; do
  git merge ${branch}
  possibly_die "Failed to merge ${branch}"
done
ok

# Try to prepare build
./scipts/prepare-build.sh
possibly_die "Failed to prepare build"

# Try to build the project
cd build/release
make
possibly_die "Failed to build the project"
cd ../..

# Rebase each branch on main
heading "# Rebasing on main"
for branch in ${BRANCHES}; do
  git switch ${branch}
  possibly_die "Failed to switch to ${branch}"

  git rebase main
  possibly_die "Failed to rebase ${branch}"
done
ok

# Push rebased branches
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
