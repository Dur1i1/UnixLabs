#!/bin/sh -e
tempDir=$(mktemp -d)
cppScript=$1
key=$2

removeTemp()
{
    errorCode=$?
    trap - EXIT
    echo "removing temp directory"
    
    if [ $errorCode != 0 ]; then
        echo "errorCode = $errorCode" 
    fi
    
    rm -rf "$tempDir"
    exit $errorCode
}
trap removeTemp EXIT TERM INT QUIT HUP

if [ ! -f "$cppScript" ]; then
    echo "file "$cppScript" doesn't exist"
    exit 1
fi

compileString=$(grep "^//$key:[[:space:]]*[[:alnum:]_]*" "$cppScript")
compileName=$(echo "$compileString" | head -1| sed "s|^//$key:[[:space:]]*||")
echo "Name to compile with: $compileName"

originDir=$(pwd)
cd "$tempDir"

g++ "$originDir/$cppScript" -o "$compileName"
cp "$compileName" "$originDir/"

echo "Success"
exit 0