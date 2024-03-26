/*
 *  Copyright (C) 2014 Steve Harris et al. (see AUTHORS)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
# include <unistd.h>
#endif
#include "lo/lo.h"

int done = 0;

void error(int num, const char *m, const char *path);

int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data);

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data);

int blobtest_handler(const char *path, const char *types, lo_arg ** argv,
                     int argc, lo_message data, void *user_data);

int pattern_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data);

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, lo_message data, void *user_data);
int aprilTagePipeBackHandler(const char* path, const char* types, lo_arg** argv,
                             int argc, lo_message data, void* user_data);
int aprilTagePipeBackALLHandler(const char* path, const char* types, lo_arg** argv,
                                int argc, lo_message data, void* user_data);
int aprilTagePipeInHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data);
int fromPhoneMultiSenseHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data);
const char* port = "9000";
const char* host = "192.168.1.3";
const char* url = "osc.udp://192.168.1.3:9000/";
lo_server_thread st;

int main()
{
    //handlers match pathes either way e.g.(server has method listining to path /f* 
    // means any path and sub paths that roots starts with /f
    // e.g.2 (client sending to path /f* will be cought with any handlers in target server that root start with /f)

    /* start a new server on port 7770 */
    st = lo_server_thread_new_multicast_iface(NULL, port, NULL, host, error);
    

    /* add method that will match any path and args */
    //lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);

    /* add method that will match the path /foo/bar, with two numbers, coerced
     * to float and int */
    lo_server_thread_add_method(st, "/foo/bar", "fi", foo_handler, NULL);

    /* add method that will match the path /blobtest with one blob arg */
    lo_server_thread_add_method(st, "/blobtest", "b", blobtest_handler, NULL);

    /* catch any message starting with /g using a pattern method */
    lo_server_thread_add_method(st, "/p*", "", pattern_handler, NULL);

    /* also catch "/q*", but glob_handle returns 1, so quit_handler
     * gets called after */
    lo_server_thread_add_method(st, "/q*", "", pattern_handler, NULL);

    /* add method that will match the path /quit with no args */
    lo_server_thread_add_method(st, "/quit", "", quit_handler, NULL);

     /* add method that will match the path /quit with no args */
    lo_server_thread_add_method(st, "/multisense/gyroscope/x", "f", fromPhoneMultiSenseHandler, NULL);//test with unity
    lo_server_thread_add_method(st, "/AprilTagPipeBack", "s", aprilTagePipeBackHandler, NULL);//test with unity
    lo_server_thread_add_method(st, "/AprilTagPipeBack", "", aprilTagePipeBackALLHandler, NULL);//test with unity
    lo_server_thread_add_method(st, "/ApriltagPipeIn/testing", "s", aprilTagePipeInHandler, NULL);//test with arpiltag
    lo_server_thread_add_method(st, "/ApriltagPipeIn/*", "s", aprilTagePipeInHandler, NULL);//test with arpiltag
    lo_server_thread_add_method(st, "/testing", "s", aprilTagePipeBackHandler, NULL);

   

    lo_server_thread_start(st);
    printf("Server Listening on Port: %s\n----------------\n", port);

    while (!done) {
#ifdef WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
    }

    lo_server_thread_free(st);

    return 0;
}

void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int generic_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data)
{
    int i;

    printf("generic handler; path: <%s>\n", path);
    for (i = 0; i < argc; i++) {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp((lo_type)types[i], argv[i]);
        printf("\n");
    }
    printf("\n");

    // get the sender info
    lo_address senderAdd = lo_message_get_source(data);
    const char* senderHostname = lo_address_get_hostname(senderAdd);
    const char* senderPort = lo_address_get_port(senderAdd);
    const char* senderURL = lo_address_get_url(senderAdd);
    int senderProto = lo_address_get_protocol(senderAdd);
    int senderTtl = lo_address_get_ttl(senderAdd); //time to live
    const char* senderIface = lo_address_get_iface(senderAdd); //network interface (returns 0 if no interface is assigned)
    //no path for sender

    // show the sender info
    printf("sender info:\nhostname: %s\nport: %s\nURL: %s\nProtocol(1 means UDP 4 is TCP): %d\nttl(if set and an UDP): %d\nIface(if not set will show 0): %s\n---------------------------------\n\n",
           senderHostname,
           senderPort,
           senderURL,
           senderProto,
           senderTtl,
           senderIface
    );

    fflush(stdout);

    return 1;
}

int foo_handler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("foo: %s <- f:%f, i:%d\n\n", path, argv[0]->f, argv[1]->i);
    fflush(stdout);

    return 0;
}
int fromPhoneMultiSenseHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data){

	 lo_address senderAddReal = lo_message_get_source(data); //make sure u used lo_send_message_from() from the april side and that this actually have the rigth april side address
    const char* senderURL = lo_address_get_url(senderAddReal);
    //no path for sender
    printf("got message from mobile app multisense! phone URL: %s on path: /multisense/gyroscope/x with val %.2f ! \n", senderURL, argv[0]->f);
	 lo_message_pp(data);

	return 0;
}
int aprilTagePipeBackHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("from unity! TO PATH: %s \nMSG: %s\n", path, &argv[0]->s);
    fflush(stdout);

    return 0;
}
int aprilTagePipeInHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("from unity! TO PATH: %s \nMSG: %s\n", path, &argv[0]->s);


    //do process

    
    // get the sender info
    lo_address senderAddReal = lo_message_get_source(data); //make sure u used lo_send_message_from() from the april side and that this actually have the rigth april side address
    lo_address senderAdd = lo_address_new("192.168.1.3", "9876");
    const char* senderURLReal = lo_address_get_url(senderAdd);
    const char* senderURL = lo_address_get_url(senderAdd);
    //no path for sender

    printf("Replying to URLReal: %s on path: /AprilTagPipeBack with 'updated' ! \n", senderURLReal);
    
    lo_send(senderAdd,"/AprilTagPipeBack", "s", "updated");
    lo_send(senderAdd, "/f*/bar", "ff", 0.12345678f, 34.0f);
    fflush(stdout);




    return 0;
}
int aprilTagePipeBackALLHandler(const char *path, const char *types, lo_arg ** argv,
                int argc, lo_message data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("from unity! TO PATH: %s \nMSG: \n", path, &argv[0]->s);
    fflush(stdout);

    return 0;
}

int blobtest_handler(const char *path, const char *types, lo_arg ** argv,
                     int argc, lo_message data, void *user_data)
{
    /* example showing how to get data for a blob */
    int i, size = argv[0]->blob.size;
    char mydata[6];

    unsigned char *blobdata = (unsigned char*)lo_blob_dataptr((lo_blob)argv[0]);
    int blobsize = lo_blob_datasize((lo_blob)argv[0]);

    /* Alternatively:
     * blobdata = &argv[0]->blob.data;
     * blobsize = argv[0]->blob.size;
     */

    /* Don't trust network input! Blob can be anything, so check if
       each character is a letter A-Z. */
    for (i=0; i<6 && i<blobsize; i++)
        if (blobdata[i] >= 'A' && blobdata[i] <= 'Z')
            mydata[i] = blobdata[i];
        else
            mydata[i] = '.';
    mydata[5] = 0;

    printf("%s <- length:%d '%s'\n", path, size, mydata);
    fflush(stdout);

    return 0;
}

int pattern_handler(const char *path, const char *types, lo_arg ** argv,
                    int argc, lo_message data, void *user_data)
{
    printf("pattern handler matched: %s\n\n", path);
    fflush(stdout);

    // Let the dispatcher continue by returning non-zero, so
    // quit_handler can also catch the message
    return 1;
}

int quit_handler(const char *path, const char *types, lo_arg ** argv,
                 int argc, lo_message data, void *user_data)
{
    done = 1;
    printf("quiting\n\n");
    fflush(stdout);

    return 0;
}

/* vi:set ts=8 sts=4 sw=4: */
