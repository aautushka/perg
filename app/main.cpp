#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "perg/perg.h"

#include "perg/mask_filter.h"

std::unique_ptr<perg::source<perg::view>> create_input(const char* filename, bool reverse)
{
	if (filename && *filename)
	{
		return std::make_unique<perg::raw_file_reader>(filename);
	}
	else
	{
		if (reverse)
		{
			return std::make_unique<perg::reverse_stdin_reader>();
		}
		else
		{
			return std::make_unique<perg::stdin_reader>();
		}
	} 
}

std::unique_ptr<perg::proc<perg::view>> create_filter(const char* mask, const char* filename, bool reverse)
{
	if (filename && *filename)
	{
		return std::make_unique<perg::filters::forward_mask_filter>(mask);
	}
	else 
	{
		return std::make_unique<perg::filters::mask_filter>(mask);
	}
}

std::unique_ptr<perg::search_result> create_output(int lines, char separator)
{
	auto out = std::make_unique<perg::search_result>();
	
	out->limit(lines);
	out->separate_by(separator);
	return out;
}

int main(int argc, char* argv[])
{
	int c = 0;
	const char* filename = "";
	const char* searchMask = "";
	bool reverse = false;
	int lines = 1000;
	char separator = '\n';

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
					"--separator,s    line separator (newline is the default)\n";
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
			case 's':
				separator = optarg[0];
				break;
			default:
				std::cout << "Invalid argument: " << (char)c << std::endl;
				return 1;
		}
	}

	perg::pipeline<perg::view> pipeline;

	auto input = create_input(filename, reverse);
	auto filter = create_filter(searchMask, filename, reverse);
	auto output = create_output(lines, separator);

	pipeline.connect(input)(filter)(*output);
	pipeline.wait();
	
	perg::stdout_stream console;
	output->dump(console);
		
	return 0;
}
