/*
 * reader1.h
 *
 * Copyright (C) 2012 - Kim Svensson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#define MAXFIELDS 7

enum arrayindex {
	UPLOADED=1,
	AUTHOR=2,
	TITLE =3,
	ID=4,
	DURATION=5,
	THUMBLOC=6
};

typedef struct entrystruct {
	char *fields[MAXFIELDS];
	void * top;
	struct entrystruct *next;
} entry;


void free_entryarray(entry *argRoot);
entry * get_rootentry(char * filename);
