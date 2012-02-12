/*
 * test2.c
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

#include<gtk/gtk.h>
#include"reader1.h"
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdint.h>
#include<time.h>

#define ENTRYWIDTH 122
#define COLOR8BIT  255
#define COLORBREAK 125
#define STARTWIDTH 1024
#define STARTHEIGHT 600
#define ENTRYHEIGHT 122

typedef struct entry_w_struct 
{
	GtkWidget * top; /*top widget to be used when added to the main window */
	GtkWidget * box;
	GtkWidget * title;
	GtkWidget * author;
	GtkWidget * duration;
	GtkWidget * thumboverlay; /*GTKoverlay to overlay author and duration over the image thumb.*/
	GtkWidget * thumb;
	entry     * thisstruct;
}entrywidget;

const GdkRGBA BLACK = {(gdouble) 0,(gdouble) 0,(gdouble) 0,(gdouble) 1};
const GdkRGBA WHITE = {(gdouble) 1,(gdouble) 1,(gdouble) 1,(gdouble) 1};
const GdkRGBA * RGBARRAY[2] = {&BLACK,&WHITE};// = {{BLACK}{WHITE}};
//const uint8_t COLORBREAK = 125;
//const uint8_t COLOR8BIT = 255;
//const uint8_t ENTRYWIDTH = 122;

unsigned int hashString(char * argstring)
{
	int n = strlen(argstring);
	int i = 0;
	uint32_t hashsum = 0;
	uint32_t tmp = 0;
	for(i = 0;i<n;i++)
	{
		//hashsum = 31 * hashsum + argstring[i]; 
		tmp = argstring[i]*pow(31,n-i);
		hashsum += tmp;
//		printf("charint %i sum %i\n",argstring[i],tmp);
	}
	//	printf("hash %u author %s lenght %i",hashsum,argstring,n);
	return hashsum;
}

GdkRGBA * getBGColor(char * argstring) {
	unsigned int hash = hashString(argstring);
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	//Argument for gdk_rgba_parse, 'rgb(255,255,255)' 16 chars max + 1 null term = 17.
	// 17 caused bug with gtk, increase to 18 solved it, dont know why.
	char tmpcolorarg[18];// = (char *) malloc(17*sizeof(char));
	if(hash<0)
		hash ^=1;
	r = hash % 255;
	hash >>=8;
	g = hash % 255;
	hash >>=8;
	b = hash % 255;
	sprintf(tmpcolorarg,"rgb(%u,%u,%u)\n",r,g,b);
	//	printf("%s",tmpcolorarg);
	GdkRGBA * retColor = (GdkRGBA *) calloc(1,sizeof(GdkRGBA));
	gdk_rgba_parse(retColor,tmpcolorarg);
	return retColor;
}

int adjustTitleColor(GdkRGBA * argColor) {
	
	uint8_t r = argColor->red*COLOR8BIT;
	uint8_t g = argColor->green*COLOR8BIT;
	uint8_t b = argColor->blue*COLOR8BIT;
	uint8_t colorBrightness = (((r*299) + (g*587) + (b*114)) / 1000);
//	printf(" brightness %i \n",colorBrightness);
	return (colorBrightness < COLORBREAK);	
}


GtkWidget * createwidget(entry * argEntry) 
{
	GtkWidget * top;
	GtkWidget * box;
	GtkWidget * title;
	GtkWidget * author;
	GtkWidget * duration;
	GtkWidget * thumboverlay; /*GTKoverlay to overlay author and duration over the image thumb.*/
	GtkWidget * thumb;
	entrywidget * eWidget; 
	GdkRGBA * bgcolor = NULL;
	uint8_t colorret =0;
	if(argEntry == (entry *) NULL)
		return (GtkWidget *) NULL;
	//construction
	top = gtk_event_box_new();
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	thumb = gtk_image_new_from_file("./default.jpg");
	title    = gtk_label_new(argEntry->title);
	author   = gtk_label_new(argEntry->author);
	duration = gtk_label_new(argEntry->duration);
	thumboverlay = gtk_overlay_new();
	eWidget->thisstruct = argEntry;	

	gtk_label_set_ellipsize(GTK_LABEL(title),PANGO_ELLIPSIZE_END);
	gtk_label_set_ellipsize(GTK_LABEL(author),PANGO_ELLIPSIZE_END);

	gtk_widget_set_size_request(thumboverlay,120,20);
	gtk_widget_set_size_request(top,ENTRYWIDTH,ENTRYHEIGHT);
	gtk_widget_set_valign(author,GTK_ALIGN_START);
	gtk_widget_set_halign(author,GTK_ALIGN_START);
	gtk_widget_set_valign(duration,GTK_ALIGN_END);
	gtk_widget_set_halign(duration,GTK_ALIGN_END);
	

	//Adding widgets
	gtk_container_add(GTK_CONTAINER(thumboverlay), thumb);
	gtk_container_add(GTK_CONTAINER(top), box);
	
	gtk_overlay_add_overlay(GTK_OVERLAY(thumboverlay),author);
	gtk_overlay_add_overlay(GTK_OVERLAY(thumboverlay),duration);

	gtk_box_set_homogeneous(GTK_BOX(box),FALSE);
	gtk_box_pack_start(GTK_BOX(box),title,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),thumboverlay,FALSE,FALSE,0);



	//Color
	bgcolor = getBGColor(argEntry->author);
	colorret = adjustTitleColor(bgcolor);
	gtk_widget_override_color(title,GTK_STATE_FLAG_NORMAL,RGBARRAY[colorret]);
	gtk_widget_override_background_color(top,GTK_STATE_FLAG_NORMAL,bgcolor);

	gtk_widget_override_background_color(thumboverlay,GTK_STATE_FLAG_NORMAL,&BLACK);
	gtk_widget_override_color(author,GTK_STATE_FLAG_NORMAL,&WHITE);
	gtk_widget_override_color(duration,GTK_STATE_FLAG_NORMAL,&WHITE);
	

	
	gtk_widget_show_all(top);
	return top;	
}


void arrange_window(entry * argentry, GtkWidget * layout,uint16_t width,
		    void (*gtk_put_move)(GtkLayout *,GtkWidget *, int,int)) 
{
	entry * currententry = argentry;
	uint8_t erowcount = 0;
	int x = 0;
	int y = 0;
	int16_t colspace = 0;
	uint8_t entryprow;

	entryprow = width/ENTRYWIDTH;
	if(entryprow < 2) {
		entryprow = 1;
		colspace = 0;
	} 
	else {
		colspace = (width -(entryprow)*ENTRYWIDTH)/(entryprow-1);
		if(colspace < 0)
			colspace = 0;
	}
	
	while(currententry->next !=NULL)
	{
		gtk_widget_hide(currententry->top);
		if(erowcount == entryprow-1)
			(*gtk_put_move)(GTK_LAYOUT(layout),currententry->top,width-ENTRYWIDTH,y);
		else
			(*gtk_put_move)(GTK_LAYOUT(layout),currententry->top,x,y);
		gtk_widget_show(currententry->top);
		currententry =(entry *) currententry->next;

		x += ENTRYWIDTH + colspace;
		erowcount++;
		if(erowcount >= entryprow)
		{
			erowcount = 0;
			x = 0;
			y += ENTRYHEIGHT + 7;
		}
	}


}


void window_resize(GtkWindow *window, GdkEvent *event, gpointer data)
{
	uint16_t width;
	uint16_t height;
	GtkWidget * layout;
	GtkWidget * currentwidget;
	entry * currententry;
	GList * windowlist;
	windowlist = gtk_container_get_children(GTK_CONTAINER(window));
	layout = (GtkWidget *) windowlist->data;
	currententry = (entry *) data;
	width = event->configure.width;
	height = event->configure.height;
	
	arrange_window(currententry,layout,width,&gtk_layout_move);

	gtk_container_resize_children(GTK_CONTAINER(layout));
	GtkRequisition size = {width,height};
	gtk_widget_size_request(layout,&size);
}


GtkWidget * getEntryGrid(entry * rootentry, GtkWidget * window) {
	
	GtkWidget * layout;
	GtkWidget * currentwidget;
	entry * currententry;
	layout = gtk_layout_new(NULL,NULL);
	currententry = rootentry;

	while(currententry->next !=NULL)
	{
		currentwidget = createwidget(currententry);
		currententry->top = currentwidget;
		currententry =(entry *) currententry->next;
	}

	arrange_window(rootentry,layout,STARTWIDTH,&gtk_layout_put);

	
	return layout;
}

int main(int argc, char *argv[]) 
{
	clock_t start = clock();
	GtkWidget * window;
	GtkWidget * layout;
	entry * rootentry = (entry *) NULL;
	rootentry = getRootentry();
	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), STARTWIDTH,STARTHEIGHT);
	gtk_widget_override_background_color(window,GTK_STATE_FLAG_NORMAL,&BLACK);
	gtk_widget_show_all(window);
//if kill
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
	g_signal_connect(window, "configure-event",G_CALLBACK(window_resize),rootentry);
	
	layout = getEntryGrid(rootentry,window);
	gtk_container_add(GTK_CONTAINER(window), layout);
	gtk_widget_show_all(layout);
	gtk_widget_show_all(window);
	printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
	gtk_main();
	return 0;

}

