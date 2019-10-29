// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders
// <vince@kyllikki.org>

#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// crawl.c ... build a graph of part of the web
#include "url_file.h"
#define BUFSIZE 1024
int main(void){
URL_FILE *handle;
char buffer[BUFSIZE];
char * firstURL = "https://www.google.com/"
if (!(handle = url_fopen(firstURL, "r")))
		{
			fprintf(stderr, "Couldn't open %s\n", next);
			exit(1);
		}

if(!url_feof(handle))
{
	url_fgets(buffer, sizeof(buffer), handle);
}

printf("%s" , buffer);
}
