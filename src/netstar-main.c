#include <netstar-commands.h>
#include <netstar-services.h>

#include <netstar-forward.h>
#include <netstar-capture.h>

#include <netstar-vendors.h>

#include <netstar-colored.h>

#include <netstar.h>

#include <console/console.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

#include <stdnoreturn.h>
#include <signal.h>

#define NETSTAR_ASCII_LARGE_LOGO_WIDTH   sizeof("                                                @%M             ")-1
#define NETSTAR_ASCII_MEDIUM_LOGO_WIDTH  sizeof("                            @               @            ")-1
#define NETSTAR_ASCII_MINIMUM_LOGO_WIDTH sizeof("                       |           ||.        ")-1


static netstar_t netstar;


int
main(int argc, char **argv) {
  netstar_print("This tool is intended solely for educational and ethical purposes. Unauthorized or improper use of this software "
                 "to exploit, attack, or compromise any systems without explicit permission is strictly prohibited.\r\n\r\n");

  netstar_print("NETSTAR * Network Stealth Traffic Analyzer & Redirector\r\n");
  netstar_print("Copyright (C) 2025, by Natan Tôrres\r\n\r\n");

/*
  netstar_services_register(netstar_arp_broadcast_spoofing_service());
  netstar_services_register(netstar_arp_network_spoofing_service());
  netstar_services_register(netstar_arp_spoofing_service());
  netstar_services_register(netstar_ndp_network_spoofing_service());
  netstar_services_register(netstar_ndp_spoofing_service());
  netstar_services_register(netstar_dns_spoofing_service());
  netstar_services_register(netstar_http_server_service());
  netstar_services_register(netstar_scanner_service());
  netstar_services_register(netstar_monitor_service());
*/

  if (console_width() > NETSTAR_ASCII_LARGE_LOGO_WIDTH) {
    netstar_print(netstar_style("bold"));
    netstar_print(
      "\r\n"
      "                                                @%M             \r\n"
      "                               %%                 @%%%          \r\n"
      "                               %M                   M%%%        \r\n"
      "                              @%%                    M%%%M      \r\n"
      "                              M%%M                    @%%%%     \r\n"
      "                              M%%M                     @%%%%M   \r\n"
      "                              %%%%                      @M%%%M  \r\n"
      "                             @%%%%@                      M%%%%M \r\n"
      "                    M        %%%%%%        M              %%%%% \r\n"
      "                      @      M%%%%M      %%                %%%%M\r\n"
      "                         M   %%%%%%   M%%%                      \r\n"
      "                      @     %%%%%%%%%%%%%@                      \r\n"
      "                       M    %%M%%M%%%%%%M                       \r\n"
      "                        @ M%%%%%%%%%%M%%                        \r\n"
      "                    @M%@ %%%%%%%%%%%%%%%M%M@                    \r\n"
      "            @M%M          %%%%%%%%%%%%%%%%%%%%%%M%M@            \r\n"
      "        %M                 %%%%%%%%%%%%%%%%%%%%%%%%%%%M%        \r\n"
      "              M%M@        %%%%%%%%%%%%%%%%%%%%MM%M              \r\n"
      "                     @M%@%%%%%%%%%%%%%%M%M@                     \r\n"
      "                           MM%%%%%%MM                           \r\n"
      "                       @    %%M  M%%    @                       \r\n"
      "                      M    @%      %@    M                      \r\n"
      "                     @   @            @   @                     \r\n"
      "MM%%%                 M      M    M      M                      \r\n"
      " %%%%%                       %    %                             \r\n"
      " @%MM%M                      @    @                             \r\n"
      "  @%%%%M                      @  @                              \r\n"
      "    %%%%M                     %  M                              \r\n"
      "     %%%%M                    M  M                              \r\n"
      "      M%%%M                                                     \r\n"
      "        M%%%                   MM                               \r\n"
      "          M%%M                 %%                               \r\n"
      "              M%                                                \r\n"
      "\r\n\r\n"
    );
    netstar_print(netstar_reset());
  } else if (console_width() > NETSTAR_ASCII_MEDIUM_LOGO_WIDTH) {
    netstar_print(netstar_style("bold"));
    netstar_print(
      "\r\n"
      "                            @               @           \r\n"
      "                            g                 &w&       \r\n"
      "                           &w                  @wwg     \r\n"
      "                           gw@                   www@   \r\n"
      "                           ww&                    www&  \r\n"
      "                          @wwg                     wgww \r\n"
      "                   @      &www@      @              www&\r\n"
      "                     &    gwww&   @w&                &&&\r\n"
      "                        &@wwwwg@www&                    \r\n"
      "                     @   gwwwwwgwww                     \r\n"
      "                      &&gwwwwwwgwg                      \r\n"
      "                @w&@   &wwwwwwwwwwwwwgw&@               \r\n"
      "         @@@@@@         &wwwwwwwwwwwwwwwwwwwwww&        \r\n"
      "               @@&&@   &wwwwwwwwwwwwwgw&@               \r\n"
      "                      @&gwwwwwwg&@                      \r\n"
      "                     @   gw@ &w@  @                     \r\n"
      "                        &@    @@@  &                    \r\n"
      " &&&                @&    @   &   @ &                   \r\n"
      " wwww              @      &   @      @                  \r\n"
      "  wwg&                    @  @                          \r\n"
      "   www&                    @ &                          \r\n"
      "    www&                   & @                          \r\n"
      "     @www                  &&                           \r\n"
      "       @ww@                 &                           \r\n"
      "           @                @                           \r\n"
      "                                                        \r\n"
      "\r\n\r\n"
    );
    netstar_print(netstar_reset());
  } else if (console_width() > NETSTAR_ASCII_MINIMUM_LOGO_WIDTH) {
    netstar_print(netstar_style("bold"));
    netstar_print(
      "\r\n"
      "                       |           ||.        \r\n"
      "                      ||             |||      \r\n"
      "                      ||               ||.    \r\n"
      "                      ||                |.|.  \r\n"
      "                      |||                |||. \r\n"
      "               |.    ||||    .|           |||.\r\n"
      "                ||.  ||||  .||                \r\n"
      "                 |||.||||.|||                 \r\n"
      "                 .||||||||||.                 \r\n"
      "             ___--||||||||||--___             \r\n"
      "       _.-||||||||||||||||||||||||||-._       \r\n"
      "                 -||||||||||-                 \r\n"
      "                 .||||||||||.                 \r\n"
      "                 |||.||||.|||                 \r\n"
      "                ||.  ||||  .||                \r\n"
      "|||.           |.    ||||    .|               \r\n"
      " |||.                 |||                     \r\n"
      "  |.|.                ||                      \r\n"
      "   .||                ||                      \r\n"
      "     |||              ||                      \r\n"
      "       .||            |                       \r\n"
      "\r\n\r\n"
    );
    netstar_print(netstar_reset());
  }

  if (netstar_command_initialize(&netstar, argc-1, argv+1) == -1)
    return 0;

 // signal(SIGINT, netstar_interrupt);

  netstar_log("running on \"%s\" interface; ready to start capturing network traffic and starting services\r\n", netstar.managed.iface->name);

  netstar_log("running using \"%s\" as loopback interface;\r\n", netstar.loopback.iface->name);

  if (netstar.forward)
    netstar_log("running on forwarding mode; all intercepted traffic will be forwarded to their respective destinations\r\n");

  netstar_threads_initialize(/* &netstar */);
  netstar_forward_initialize(/* &netstar */);

  netstar_vendors_initialize(/* &netstar */);

  netstar_services_initialize(&netstar);
// netstar_modules_initialize(&netstar);

  netstar_capture_start(&netstar);

  if (!netstar.interrupted)
    while (toupper(getchar()) != 'E');

  netstar.interrupted = true;

  if (netstar.interrupted)
    netstar_log("interrupted; stopping/deinitializing services...\r\n");

  netstar_terminate();
  return 0;
}

netstar_t *
netstar_context(void) {
  return &netstar;
}

void
netstar_close(netstar_t *netstar) {
#if defined __linux__ || defined __linux
  netstar_linkwave_free(&netstar->loopback4);
  netstar_linkwave_free(&netstar->loopback6);
#endif
  netstar_linkwave_free(&netstar->loopback);

  netstar_linkwave_free(&netstar->managed);
}
