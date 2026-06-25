#!/bin/sh

IMAGE_NAME=infoflow:latest

do_help () {
    echo "Build project in a fresh Docker image with tag \`infoflow:latest\`"
    echo
    echo "Options:"
    printf "\t-h                   Display this help\n"
    printf "\t-r                   Start an ephemeral container after build\n"
    printf "\t-c                   Build a fresh image\n"
}

flags=""
do_run=false
while getopts "rhc" opt
do
    case $opt in
        h) do_help; exit 0;;
        r) do_run=true ;;
        c) flags="$flags --no-cache" ;;
        ?) echo "Invalid option -$OPTARG" >&2; exit 1;;
    esac
done

cmd="docker build -t $IMAGE_NAME $flags ."
echo "$cmd"
$cmd

if [ $do_run = true ]
then
    docker run --rm -ti $IMAGE_NAME
fi
