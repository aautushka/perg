#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "perg/perg.h"
#include "perg/mask_filter.h"

int main(int argc, char* argv[])
{
	int c = 0;
	const char* filename = "";
	const char* searchMask = "";

	while ((c = getopt(argc, argv, "hi:m:")) != -1)
	{
		switch (c)
		{
			case 'h':
				break;
			case 'i':
				filename = optarg;
				break;
			case 'm':
				searchMask = optarg;
				break;
			default:
				std::cout << "Invalid argument: " << (char)c << std::endl;
				exit(1);
		}
	}

	perg::mypipeline<perg::view> pipeline;
	perg::file_reader file(filename);
	perg::filters::mask_filter mask(searchMask);
	perg::search_result result;

	pipeline.connect(file)(mask)(result);
	pipeline.wait();

	perg::stdout_stream stream;
	result.dump(stream);
		
	return 0;
}
