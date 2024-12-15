#!/bin/sh

BRANCHES="dev-ak dev-jm dev-jr dev-mb dev-mm dev-mp"

# Pull newest changes in each branch
echo "# Pulling changes"
for branch in ${BRANCHES}; do
  git switch ${branch}
  git pull
done
echo "... done"
echo ""

# Merge each branch to main
echo "# Merging to main"
git switch main
for branch in ${BRANCHES}; do
  git merge ${branch}
done
echo "... done"
echo ""

# Rebase each branch on main
echo "# Rebasing on main"
for branch in ${BRANCHES}; do
  git switch ${branch}
  git rebase main
done
echo "... done"
echo ""

# TODO verify build

# Push rebased branches
echo "# Pushing rebased branches"
for branch in ${BRANCHES}; do
  git switch ${branch}
  git push
done
echo "... done"
echo ""

# Push main
git switch main
git push

# Switch back home
git switch dev-mm
