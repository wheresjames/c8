
function(get_latest_stable_git_tag REPO_URL RESULT_VAR)
    execute_process(
        COMMAND ${CMAKE_SOURCE_DIR}/sh/get-latest-stable-git-tag.sh ${REPO_URL}
        OUTPUT_VARIABLE result
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (result)
        set(${RESULT_VAR} ${result} PARENT_SCOPE)
    else()
        message(FATAL_ERROR "Failed to get latest stable tag from ${REPO_URL}")
    endif()
endfunction()
