#
#  Taken from Zed Shaw's awesome 'Learn to Code C the Hard Way': http://c.learncodethehardway.org/book
#  and modified a smidge
#

echo "\n\nRunning unit tests:"

for i in $(find ./tests -name "*_tests" -type f -maxdepth 4) $tests/*_tests
do
    if test -f $i
    then
        if $VALGRIND ./$i 2>> tests/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's tests/tests.log"
            echo "------"
            tail tests/tests.log
            exit 1
        fi
    fi
done

echo ""
