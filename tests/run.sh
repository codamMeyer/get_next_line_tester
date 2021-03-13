PURPLE="\e[1;95m"
RESET="\e[0m"
GREEN="\e[1;32m"
RED="\e[1;31m"
COLS=$(tput cols)
RESULT=0

function run_norminette()
{

	print_small_banner "NORM CHECK"

	norminette yourFilesHere/get_next_line.c
	norminette yourFilesHere/get_next_line_utils.c
	norminette yourFilesHere/get_next_line.h

	if $WITH_BONUS ; then
		norminette yourFilesHere/get_next_line_bonus.c
		norminette yourFilesHere/get_next_line_utils_bonus.c
		norminette yourFilesHere/get_next_line_bonus.h
	fi
}

function print_big_banner()
{
	printf "${GREEN}%80s\n%80s\n" |tr " " "="
	figlet -c  $1
	printf "${GREEN}%80s\n%80s\n${RESET}" | tr " " "="
	sleep 1
}

function print_small_banner()
{
	echo ""
	printf "\n${PURPLE}%80s\n${RESET}" | tr " " "="
	printf "%*s" $[COLS/3,5] "				$1"
	printf "\n${PURPLE}%80s\n${RESET}" | tr " " "="
}

function print_warning()
{
	printf "\n\n${RED}%80s\n${RESET}" | tr " " "*"
	printf "	           $1"
	printf "\n${RED}%80s\n\n${RESET}" | tr " " "*"
}


function print_success()
{
	printf "\n${GREEN}%80s\n%80s\n"  |tr " " "*"
	printf "\n*%78s*\n"
	figlet -c  "*   $1   *"
	printf "\n*%78s*\n"
	printf "\n${GREEN}%80s\n%80s\n"   |tr " " "*"
}

function test_stdin()
{
	make clean > /dev/null && make stdin SIZE=42  > /dev/null

	echo "Hello World!" | ./test > /dev/null

	printf "hello this is a test\nwith more than one line\nhehe\n" | ./test  > /dev/null

	echo "answer to life the universe and everythin" | ./test > /dev/null

	echo "answer to life the universe and everything" | ./test > /dev/null

	echo "answer to life the universe and everything-" | ./test > /dev/null

	cat ./tests/text/alphabet | ./test > /dev/null
	cat ./tests/text/file_without_nl | ./test > /dev/null
	cat ./tests/text/empty_lines | ./test > /dev/null

	if diff -a --suppress-common-lines -y ./tests/results/expected_stdin ./tests/results/actual_stdin  > ./tests/results/result_stdin 2>&1
	then
		print_small_banner "STDIN BUFFER SIZE 42"
		echo ""
		echo -e "TEST 1/8 stdin_test:basic_line $GREEN[OK]$RESET"
		echo -e "TEST 2/8 stdin_test:multiple_lines $GREEN[OK]$RESET"
		echo -e "TEST 3/8 stdin_test:41_chars $GREEN[OK]$RESET"
		echo -e "TEST 4/8 stdin_test:42_chars $GREEN[OK]$RESET"
		echo -e "TEST 5/8 stdin_test:43_chars $GREEN[OK]$RESET"
		echo -e "TEST 6/8 stdin_test:alphabet $GREEN[OK]$RESET"
		echo -e "TEST 7/8 stdin_test:file_without_nl $GREEN[OK]$RESET"
		echo -e "TEST 8/8 stdin_test:empty_file $GREEN[OK]$RESET"
		echo ""
	else
		echo -e "$RED[FAIL]$RESET stdin failed, check result_stdin file\n"
		RESULT=$((RESULT+1))
	fi
}

print_big_banner "GNL TESTER"


print_small_banner "BUFFER SIZE 0"

make clean > /dev/null && make test_invalid_size > /dev/null
./test invalid
RESULT+=$?

################################################################################
#      YOU CAN EDIT THIS PART TO ADD OR DELETE BUFFER SIZES TO BE TESTED       #
################################################################################
for i in 2 3 2000 99999 100000 2000000 42
do
	print_small_banner "BUFFER SIZE $i"
	make fclean > /dev/null && make test SIZE=$i > /dev/null
	./test
	RESULT=$((RESULT+$?))
done

test_stdin

if [[ $1 == bonus ]]; then

	print_big_banner "GNL BONUS TESTER"
	for i in 2 3 2000 99999 10000000 2000000000
	do
		print_small_banner "BONUS BUFFER SIZE $i"
		make clean > /dev/null && make bonus SIZE=$i > /dev/null
		./test
		RESULT=$((RESULT+$?))
	done

	test_stdin
fi

if [[ $RESULT == 0 ]]; then
	print_success "ALL GOOD!"
else
	print_warning "Some tests failed, please take a look"
fi

make clean  > /dev/null

################################################################################
#              COMMENT THS PART TO SKIP NORM CHECK                             #
run_norminette                                                                #
#                                                                              #
################################################################################

echo ""

