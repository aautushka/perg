#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "perg/perg.h"

#include "perg/mask_filter.h"

int main(int argc, char* argv[])
{
	int c = 0;
	const char* filename = "";
	const char* searchMask = "";
	bool reverse = false;
	int lines = 1000;

	struct option opts[] = {
		{ "help",      no_argument,       0, 'h'},
		{ "file_path", required_argument, 0, 'f'},
		{ "mask",      required_argument, 0, 'm'},
		{ "max_lines", required_argument, 0, 'l'},
		{ "scan_tail", no_argument,       0, 'r'},
		{ "separator", required_argument, 0, 's'},
		{ 0, 0, 0, 0}};

	int optindex = 0;
	while ((c = getopt_long(argc, argv, "hf:m:l:rs:", opts, &optindex)) != -1)
	{
		switch (c)
		{
			case 'h':
				std::cout <<
				       	"perg - the grep defiled\n\n"
					"Options\n"
					"--help,h         print this message\n"
					"--file_path,f    input file; stdin if no file provided\n"
					"--mask,m         search mask (*? are allowed\n"
					"--max_lines,l    max result lines, default is 1000\n"
					"--scan_tail,r    read file backwards\n"
					"--separator,s    found lines separator\n";
				return 0;
			case 'f':
				filename = optarg;
				break;
			case 'm':
				searchMask = optarg;
				break;
			case 'r':
				reverse = true;
				break;
			case 'l':
				lines = std::min(1000, atoi(optarg));
				break;
				
			default:
				std::cout << "Invalid argument: " << (char)c << std::endl;
				return 1;
		}
	}

	perg::pipeline<perg::view> pipeline;
	perg::filters::mask_filter mask(searchMask);
	perg::search_result result;

	result.limit(lines);
	
	if (*filename)
	{
		if (reverse)
		{
			perg::reverse_file_reader file(filename);
			pipeline.connect(file)(mask)(result);
			pipeline.wait();
		}
		else
		{
			perg::file_reader file(filename);
			pipeline.connect(file)(mask)(result);
			pipeline.wait();
		}
	}
	else
	{
		if (reverse)
		{
			perg::reverse_stdin_reader source;
			pipeline.connect(source)(mask)(result);
			pipeline.wait();
		}
		else
		{
			perg::stdin_reader source;
			pipeline.connect(source)(mask)(result);
			pipeline.wait();
		}
	} 

	perg::stdout_stream stream;
	result.dump(stream);
		
	return 0;
}
