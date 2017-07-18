#!/bin/bash

STORAGE="${HOME}/.configsets"
DEBUG=1

#: Config file management utility
#:
#: Usage:
#: configset.sh COMMAND ARGUMENTS
#:
#: Available commands:
#: add FILENAME SETNAME - add a file to specified config set
#: ls SETNAME - print all files in specified config set
#: restore SETNAME - restore files from specified config set
#: sets - print existing config sets
#: help - print this message

mkdir -p "${STORAGE}"

help() {
    grep '^#:' "$(realpath $0)" | perl -pe 's;^#:;;'
    exit 0
}

_debug() {
    test "x${DEBUG}x" == "x1x" && echo "$@"
}

add_file() {
    file_name="$1"
    set_name="$2"
    ( test "x${file_name}x" == "xx" || test "x${set_name}x" == "xx" ) && help
    file_path=$(realpath "${file_name}")
    file_name=$(basename "${file_path}")
    dir_name=$(dirname "${file_path}")
    dest_file_name="__${set_name}__${file_name}"
    mkdir -p "${STORAGE}${dir_name}"
    _debug "COPY: ${file_path} -> ${STORAGE}${dir_name}/${dest_file_name}"
    cp -f "${file_path}" "${STORAGE}${dir_name}/${dest_file_name}"
}

print_files() {
    set_name="$1"
    test "x${set_name}x" == "xx" && help
    _debug "FILES IN '${set_name}':"
    find "${STORAGE}" -type f -iname "__${set_name}__*" \
        | perl -pe "s;^${STORAGE};;;s;__${set_name}__;;"
}

restore_files() {
    set_name="$1"
    test "x${set_name}x" == "xx" && help
    for storage_file in `find "${STORAGE}" -type f -iname "__${set_name}__*"`; do
        dest_file=$(echo "${storage_file}" | perl -pe "s;^${STORAGE};;;s;__${set_name}__;;" )
        _debug "COPY: ${storage_file} -> ${dest_file}"
        cp -f "${storage_file}" "${dest_file}"
    done
}

print_sets() {
    _debug "Available config sets:"
    find "${STORAGE}" -type f -exec basename {} \; \
        | perl -pe 's;^__([^_]+)__.+;$1;' \
        | sort \
        | uniq
}

parse_args() {
    case "$1" in
        add)
            add_file "$2" "$3" # file, config set
            ;;
        ls)
            print_files "$2" # file, config set
            ;;
        restore)
            restore_files "$2" # config set
            ;;
        sets)
            print_sets
            ;;
        help)
            help
            ;;
        *)
            help
        ;;
    esac
}

parse_args $@
