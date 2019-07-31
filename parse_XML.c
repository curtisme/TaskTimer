#include <stdlib.h>
#include <string.h>
#include "parse_XML.h"

static int is_prefix(char *s1, char *s2, int *offset);
static int string_search(char *s1, char *s2, int *index);
static int get_open_tag(char *s, char *new, int *offset);

int
is_prefix(char *s1, char *s2, int *offset) {
	int i;
	i = 0;
	while (*(s1+i) && *(s2+i)) {
		*offset = i;
		if (*(s1+i) != *(s2+i))
			return 0;
		i++;
	}
	*offset = i;
	if (!*(s1+i))
		return 1;
	return 0;
}

int
string_search(char *s1, char *s2, int *index) {
	int i,offset;
	i=0;
	if (!*s1)
		return 1;
	while (*(s2+i)) {
		*index = i;
		if (is_prefix(s1, s2+i, &offset))
			return 1;
		i += offset;
		if (!*(s2+i))
			return 0;
		i++;
	}
	return 0;
}

int
get_open_tag(char *s, char *new, int *offset) {
	int i,j;
	i=0;
	j=0;
	while (1) {
		if (!*(s+i))
			return 0;
		if (*(s+i) == '<')
			break;
		i++;
	}
	i++;
	while (1) {
		if (!*(s+i))
			return 0;
		if (*(s+i) == '>') {
			if (j==0)
				return 0;
			*offset = i;
			break;
		}
		new[j++] = s[i++];

	}
	new[j] = '\0';
	return 1;
}


int
get_tag(char *s, char *name, char *contents, char **rest) {
	int i,j,len;
	char *close_tag;
	if (!get_open_tag(s, name, &i))
		return 0;
	len = strlen(name);
	if (!(close_tag = malloc(len + 4)))
		return 0;
	close_tag[0] = '<';
	close_tag[1] = '/';
	strcpy(close_tag+2, name);
	close_tag[len+2] = '>';
	close_tag[len+3] = '\0';
	if (!string_search(close_tag, s, &j)) {
		free(close_tag);
		return 0;
	}
	strncpy(contents, s + (i+1), j-i-1);
	contents[j-i-1] = '\0';
	*rest = s+j+len+3;
	free(close_tag);
	return 1;
}

