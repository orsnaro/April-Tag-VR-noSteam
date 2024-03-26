
#include <iostream>
#include <atomic>

#ifndef WIN32
#include <unistd.h>
#endif

#include <lo/lo.h>
#include <lo/lo_cpp.h>
//#include"OSCtypeGetter.h" 
std::atomic<int> received(0);

#pragma region TO_BE_MOVED
///// TODO: move it to OSCtypeGetter.h and .c
//#define PRINTF_LL "ll"
//typedef union {
//    int32_t i;
//    float f;
//    char c;
//    uint32_t nl;
//} lo_pcast32;
//
//typedef union {
//    int64_t i;
//    double f;
//    uint64_t nl;
//} lo_pcast64;
///// TODO: move it to OSCtypeGetter.h and .c
//void Our_OSC_type_getter(lo_type type, void* data, int bigendian) {
//    lo_pcast32 val32;
//    lo_pcast64 val64 = { 0 };
//    lo_timetag valtt = { 0, 1 };
//    int size;
//    int i;
//
//    size = (int)lo_arg_size(type, data);
//    if (size == 4 || type == LO_BLOB) {
//        if (bigendian) {
//            val32.nl = lo_otoh32(*(int32_t*)data);
//        } else {
//            val32.nl = *(int32_t*)data;
//        }
//    } else if (type == LO_TIMETAG) {
//        valtt.sec =
//            bigendian ? lo_otoh32(*(uint32_t*)data) : *(uint32_t*)data;
//        data = (uint32_t*)data + 1;
//        valtt.frac =
//            bigendian ? lo_otoh32(*(uint32_t*)data) : *(uint32_t*)data;
//    } else if (size == 8) {
//        if (bigendian) {
//            val64.nl = lo_otoh64(*(int64_t*)data);
//        } else {
//            val64.nl = *(int64_t*)data;
//        }
//    }
//
//    switch (type) {
//    case LO_INT32:
//        printf("%d", val32.i);
//        break;
//
//    case LO_FLOAT:
//        printf("%f", val32.f);
//        break;
//
//    case LO_STRING:
//        printf("\"%s\"", (char*)data);
//        break;
//
//    case LO_BLOB:
//        printf("[");
//        if (val32.i > 12) {
//            printf("%d byte blob", val32.i);
//        } else {
//            printf("%db ", val32.i);
//            for (i = 0; i < val32.i; i++) {
//                printf("%#02x", (unsigned int)*((unsigned char*)(data)+4 + i));
//                if (i + 1 < val32.i)
//                    printf(" ");
//            }
//        }
//        printf("]");
//        break;
//
//    case LO_INT64:
//        printf("%" PRINTF_LL "d", (long long int) val64.i);
//        break;
//
//    case LO_TIMETAG:
//        printf("%08x.%08x", valtt.sec, valtt.frac);
//        break;
//
//    case LO_DOUBLE:
//        printf("%f", val64.f);
//        break;
//
//    case LO_SYMBOL:
//        printf("'%s", (char*)data);
//        break;
//
//    case LO_CHAR:
//        printf("'%c'", (char)val32.c);
//        break;
//
//    case LO_MIDI:
//        printf("MIDI [");
//        for (i = 0; i < 4; i++) {
//            printf("0x%02x", *((uint8_t*)(data)+i));
//            if (i + 1 < 4)
//                printf(" ");
//        }
//        printf("]");
//        break;
//
//    case LO_TRUE:
//        printf("#T");
//        break;
//
//    case LO_FALSE:
//        printf("#F");
//        break;
//
//    case LO_NIL:
//        printf("Nil");
//        break;
//
//    case LO_INFINITUM:
//        printf("Infinitum");
//        break;
//
//    default:
//        fprintf(stderr, "liblo warning: unhandled type: %c\n", type);
//        break;
//    }
//}

#pragma endregion


// lo_method_handler  is essentially a int but in typedef...
int  example_handler2(const char* path, const char* types, lo_arg** argv,
                    int argc, lo_message data, void* user_data) 
{
    using namespace std;

    //if ok inc the received_cnt
    received++;

    cout << "--------\n\ncustom example_handler2:\n";
    cout << "received a message! from:" << path << endl;
    cout << "contents: " << endl;
    fflush(stdout);
    for (int i = 0; i < argc; i++) {
        if (types[i] == 's' || types[i] == 'S') {
            cout << "type(" << types[i] << ") " << "content: ";
            cout << &argv[i]->s << endl;
        } else {
            cout << "type(" << types[i] << ") " << "content: ";
            lo_arg_pp((lo_type)types[i], argv[i]); //prints the type (TODO: a function to check for the type string and gets the variable as string  not print it!) then i save it in auto var)
            cout << endl;
        }
    }
    return 0;
}

//orsnaro note: example using this function pointer 
lo_method_handler example = &example_handler2; //now can use example instead of the other name

int main()
{
    const lo::string_type _tstIp = "192.168.1.3";
    const lo::num_string_type _port = "9000";

    /*
     * Create a server on a background thread.  Note, all message
     * handlers will run on the background thread!
     * uses local host default ip
     */
    //lo::ServerThread st(9000); // ServerThread inherits from Server which inhertis the base: server

    //orsnaro: CUSTOM IP (i made the constructor) 
    lo::ServerThread st(_port, _tstIp); 
        std::cout << "Nope." << std::endl;
        return 1;
    }
    //orsnaro: CUSTOM IP2 (this constructor is theri (multicast server thread)
    ///TODO: try multicast??https://tldp.org/HOWTO/Multicast-HOWTO-2.html and https://www.winsocketdotnetworkprogramming.com/clientserversocketnetworkcommunication8k.html#:~:text=When%20joining%20a%20group%2C%20the,all%20on%20the%20same%20socket.
    lo::ServerThread st(NULL, _port, NULL, _tstIp, NULL); 
    if (!st.is_valid()) {
        std::cout << "Nope." << std::endl;
        return 1;
    }

    /* Set some lambdas to be called when the thread starts and
     * ends. Here we demonstrate capturing a reference to the server
     * thread. */
    st.set_callbacks([&st](){printf("Thread init: %p.\n",&st);},
                     [](){printf("Thread cleanup.\n");});

    std::cout << "Starting server with URL: " << st.url() << std::endl;
  

    /*
     * Counter for number of messages received -- we use an atomic
     * because it will be updated in a background thread.
     */

    /*
     * Add a method handler for "/example,i" using a C++11 lambda to
     * keep it succinct.  We capture a reference to the `received'
     * count and modify it atomatically.
     *
     * You can also pass in a normal function, or a callable function
     * object.
     *
     * Note: If the lambda doesn't specify a return value, the default
     *       is `return 0', meaning "this message has been handled,
     *       don't continue calling the method chain."  If this is not
     *       the desired behaviour, add `return 1' to your method
     *       handlers.
     */
    st.add_method("/example", "i",
                  [&received](lo_arg** argv, int argc) {std::cout << "example (" << (++received) << "): "
                  << "received: " << argv[0]->i << std::endl; fflush(stdout); });

    //using normal function as a handler (you can also used the one defined via func pointer 'example()`
     st.add_method("/example2", NULL, example_handler2, NULL);

    /*
     * Start the server.
     */
    st.start();

    /*
     * Send some messages to the server we just created on localhost.
     */
    lo::Address a("192.168.1.3", "90010");


    /*
     * An individual message
     */
    a.send("/example", "i", 7890987);

    /*
     * Initalizer lists and message constructors are supported, so
     * that bundles can be created easily:
     */
    a.send(lo::Bundle({{"/example", lo::Message("i", 1234321)},
                       {"/example", lo::Message("i", 4321234)}}));

    /*
     * Polymorphic overloads on lo::Message::add() mean you don't need
     * to specify the type explicitly.  This is intended to be useful
     * for templates.
     */
    //lo::Message m("sccs", "update tracker with ", 'i','d'," 4 val: (x, 5)(y, 4)((z, 5) (0, 0, 0)");
    lo::Message m;
    m.add_string("update tracker with ");
    m.add_char('i'); //overloads function most variables but not string
    m.add_char('d'); //overloads function most variables but not string
    m.add_string(" 4 val: (x,5)(y,4)((z,5) (0,0,0)");
    int send_state= a.send("/example2", m);
    std::cout << "custom example send state (-1fail or size): " << send_state << std::endl;
    //lo_arg **tstargv = m.argv();
    //std::cout << &(tstargv[0]->s) << std::endl; //use dereference '&' with string 's' 'S'
    //std::cout << (tstargv[1]->c);
    //std::cout << (tstargv[2]->c);
    //std::cout << &(tstargv[3]->s) << std::endl;

    /*
      Wait for messages to be received and processed.
     */
    int tries = 500;
    while (received < 4 && --tries > 0) {
#ifdef WIN32
        Sleep(10);
#else
        usleep(10*1000);
#endif
    }

    if (tries <= 0) {
        std::cout << "Error, waited too long for messages." << std::endl;
        return 1;
    }

    /*
     * Resources are freed automatically, RAII-style, including
     * closing the background server.
     */
    std::cout << "Success!" << std::endl;
}
