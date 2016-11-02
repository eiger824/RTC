#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <string.h>
#include <getopt.h>
#include <fstream>

int max_month_days[12] = {31,
			  0, //will be filled on runtime
			  31,
			  30,
			  31, 
			  30,
			  31,
			  31,
			  30,
			  31,
			  30,
			  31};
static const std::string months[12] =  {"January",
					"February",
					"March",
					"April",
					"May",
					"June",
					"July",
					"August",
					"September",
					"October",
					"November",
					"December"};
static const std::string VERSION = "1.1";
static const std::string AUTHOR = "Santi Pagola";

bool isLeap(int year)
{
	if (year % 4 != 0)
	{
		return false;
	}
	else if (year % 100 != 0)
	{
		return true;
	}
	else if (year % 400 != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int showVer()
{
	printf("RealTimeClock version %s\n",VERSION.c_str());
	printf("Developed by %s on 20/10/2016 (which is leap year ;-)\n", AUTHOR.c_str());
	return 0;
}
int help(int code)
{
	printf("RealTimeClock (rtc) sample program.\nUSAGE: rtc [OPTIONS] {<year> | <date>}\n");
	printf("[OPTIONS]\n");
	printf("-d\t\tEnable debug messages\n");
	printf("-D <date>\tInput a date (dd-mm-yy) to output weekday\n");
	printf("-f\t\tSilent, don't print calendar to stdout\n");
	printf("-h\t\tPrint this help and exit\n");
	printf("-s\t\tSave output to file\n");
	printf("-v\t\tShow version and author and exit\n");
	printf("-y <year>\tInput a year to output calendar\n");
	if (code == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
int main(int argc, char* argv[]) {
	if (argc == 1)
	{
		printf("No arguments\n");
		return help(1);
	}
	bool debug = false;
	bool save = false;
	bool silent = false;
	int opt;
	int year = -1;
	std::string date;
	while ((opt = getopt(argc, argv, "dhfsvD:y:")) != -1)
	{
		switch (opt)
		{
			case 'd':
				debug = true;
				printf("Debug enabled\n");
			break;
			case 'y':
				year = atoi(optarg);
			break;
			case 'v':
			return showVer();
			case 'h':
			return help(0);
			case 'f':
				silent = true;
			break;
			case 's':
				save = true;
				printf("Will save to file\n");
			break;
			case 'D':
				date = optarg;
				printf("Input date: %s", date.c_str());
			break;
			default:
				printf ("Unknown option\n");
			return help(1);
		}
	}
	if (year == -1 && date == "")
	{
		printf("Neither year of date was input\n");
		return help(1);
	}
	std::string filename;
	std::ofstream file;
	if (save)
	{
		if (year != -1)
		{	filename = std::to_string(year) + ".txt";
		}
		else
		{
			filename = date;
		}
		file.open(filename.c_str());
	}
	int init = 1900;
	if (isLeap(year))
	{
		max_month_days[1] = 29;
	}
	else
	{
		max_month_days[1] = 28;
	}
	bool leap = isLeap(year);
	
	// Find out how many days till beginning of given year since init
	int nr = 0;
	for (unsigned c=init; c < year; ++c)
	{
		if (isLeap(c))
		{
			nr += 366;
		}
		else
		{
			nr += 365;
		}
	}
	int offset = nr % 7;
	if (debug)
	{
		printf("(Will start 1st jan in day number: %d [1-7])\n", offset+1);
	}
	/* Generate calendar according to input year */
	if (!silent)
	{
		printf("Calendar for year %d\n", year);
	}
	/* Keep track of weekday */
	int wnr;
	if (offset == 0)
	{
		wnr = 1;
	}
	else
	{
		wnr = 0;
	}
	// Loop through months
	for (unsigned i=1; i<=12; ++i)
	{
		if (!silent)
		{
			printf("%s\n------------\n",months[i-1].c_str());
		}
		if (save)
		{
			file << months[i-1] << "\n------------\n";	
		}
		// Loop through days
		for (unsigned j=1; j <= max_month_days[i-1]; ++j)
		{
			if (j==1)
			{
				if (!silent)
				{
					printf("%d)\t",wnr);
				}
				
				if (save)
				{
					file << wnr << ")\t";
				}
				for  (unsigned k=0; k < offset % 7; ++k)
				{
					if (!silent)
					{
						printf("\t");
					}
					if (save)
					{
						file << "\t";
					}
				}
			}
			if (offset % 7 == 6 ||
			   j == max_month_days[i-1]) 
			{
				if (j < max_month_days[i-1])
				{
					++wnr;
					if (!silent)
					{
						printf("%d\n%d)\t", j, wnr);
					}
					if (save)
					{
						file << j << "\n" << wnr << ")\t";
					}
				}
				else
				{
					if (!silent)
					{					
						printf("%d\n", j);
					}
					if (save)
					{
						file << j << "\n";
					}
				}
				if (offset % 7 == 6 &&
				   j == max_month_days[i-1])
				{
					++wnr;
					if (!silent)
					{
						printf("\n");
					}
					if (save)
					{
						file << "\n";
					}
				}
			}
			else
			{
				if (!silent)
				{
					printf("%d\t", j);
				}
				if (save)
				{
					file << j << "\t";
				}
			}
			// Reset modular offset
			++offset;	
		}
	}
	if (save)
	{
		file.close();
		printf("Saved to file!\n");
	}
	return 0;
}
