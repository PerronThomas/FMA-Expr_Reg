all : Expression input.txt automate.txt

Expression : main.cpp 
	rm -f output.txt
	g++ $^ -o $@

automate.txt : Expression
	./$^

clean:
	rm -f Expression output.txt automate.txt etat.txt transition.txt
