
if [ "$1" == "" ]; then
    printf  "ERROR: First Arg is empty, Please Provide Commit Comment"
else
        git add .
        git commit -m "$1"
        git push origin master
        printf "Code Comitted. Please Verify\n"
fi
