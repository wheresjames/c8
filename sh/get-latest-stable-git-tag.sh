#!/usr/bin/env bash

get_latest_stable_git_tag()
{
    local GITURL=$1

    # Get all tags, filter only x.y.z with patch < 10000 (filters out snapshot builds)
    all_versions=$(git ls-remote --tags ${GITURL} | \
        grep -o 'refs/tags/[0-9]\+\.[0-9]\+\.[0-9]\+$' | \
        sed 's|refs/tags/||' | \
        awk -F. '$3 < 10000' | \
        sort -t. -k1,1n -k2,2n -k3,3n)

    # Get unique sorted major.minor versions
    mapfile -t minors < <(echo "$all_versions" | cut -d. -f1,2 | sort -t. -k1,1n -k2,2n | uniq)

    if (( ${#minors[@]} < 2 )); then
        echo "Not enough stable versions found." >&2
        return 1
    fi

    # Pick the second-to-last minor (previous stable line)
    target_minor="${minors[-2]}"

    # Now get the highest patch for that minor
    latest=$(echo "$all_versions" | grep "^${target_minor}\." | tail -n1)

    echo "$latest"
}

# If this script is called with a git URL
GITURL=$1
if [ ! -z "${GITURL}" ]; then
    echo $(get_latest_stable_git_tag ${GITURL})
fi
