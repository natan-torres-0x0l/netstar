#include "netstar-stealth.h"

#include <network/sockets/sockets.h>

#include <strings/strings.h>
#include <string.h>

#include <netstar-time.h>


typedef enum {
  NETSTAR_STEALTH_INTERFACE_STATUS_UP,
  NETSTAR_STEALTH_INTERFACE_STATUS_DOWN
} netstar_stealth_interface_status_t;


#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
// #include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <initguid.h>

DEFINE_GUID(GUID_DEVCLASS_NET, 0x4d36e972, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18);

#include <stdio.h>

#define NETSTAR_CONTROL_NETWORK_ADAPTERS_CLASS "{4D36E972-E325-11CE-BFC1-08002BE10318}"

// reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Class\{4d36e972-e325-11ce-bfc1-08002be10318}\0001" /v NetworkAddress /t REG_SZ /d E89EB465684F /f
// netsh interface set interface <iface> Disable
// netsh interface set interface <iface> Enable

static int
netstar_stealth_interface_set_status(struct netlink_interface *iface, netstar_stealth_interface_status_t status) {
  SP_DEVINFO_DATA setup_device_info_data = {0};
  HDEVINFO handle_devices_info = {0};

  DWORD member_index;

  DEVINST device_instance;

  int returned = -1;

  if ((handle_devices_info = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT)) == INVALID_HANDLE_VALUE)
    return -1;

  setup_device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

  for (member_index = 0; SetupDiEnumDeviceInfo(handle_devices_info, member_index, &setup_device_info_data); member_index++) {
    BYTE adapter_name[1024];
    DWORD adapter_name_size = sizeof(adapter_name);

    HKEY registry_key = NULL;

    if ((registry_key = SetupDiOpenDevRegKey(handle_devices_info, &setup_device_info_data, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_READ)) == INVALID_HANDLE_VALUE)
      continue;

    if (RegQueryValueEx(registry_key, "NetCfgInstanceId", NULL, NULL, adapter_name, &adapter_name_size) != ERROR_SUCCESS) {
      RegCloseKey(registry_key);
      continue;
    }

    RegCloseKey(registry_key);

    if (!string_equals((const char *)adapter_name, iface->adapter_name, true))
      continue;

    device_instance = setup_device_info_data.DevInst;

    if (status == NETSTAR_STEALTH_INTERFACE_STATUS_UP && CM_Enable_DevNode(device_instance, 0) != CR_SUCCESS)
      break;

    if (status == NETSTAR_STEALTH_INTERFACE_STATUS_DOWN && CM_Disable_DevNode(device_instance, 0) != CR_SUCCESS)
      break;

    returned = 1;
    break;
  }

  SetupDiDestroyDeviceInfoList(handle_devices_info);
  return returned;
}

static int
netstar_stealth_interface_macieee802(struct netlink_interface *iface, const network_macaddr_t *stealth_mac) {
  char stealth_mac_value[NETWORK_MACADDR_STRLENGTH] = {0}, registry_path[256] = {0};
  HKEY registry_key = NULL, registry_subkey = NULL;

  TCHAR registry_enum_key[1024];
  DWORD registry_enum_key_size = sizeof(registry_enum_key);

  BYTE registry_query_value[1024];
  DWORD registry_query_value_size = sizeof(registry_query_value);

  int returned = -1;
  DWORD index;

  snprintf(registry_path, sizeof(registry_path), "SYSTEM\\CurrentControlSet\\Control\\Class\\" NETSTAR_CONTROL_NETWORK_ADAPTERS_CLASS);

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registry_path, 0, KEY_ALL_ACCESS, &registry_key) != ERROR_SUCCESS)
    return -1;

  snprintf(stealth_mac_value, sizeof(stealth_mac_value), "%02X%02X%02X%02X%02X%02X", stealth_mac->u8[0], stealth_mac->u8[1], stealth_mac->u8[2], stealth_mac->u8[3], stealth_mac->u8[4], stealth_mac->u8[5]);

  for (index = 0; RegEnumKeyEx(registry_key, index, registry_enum_key, &registry_enum_key_size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; index++) {
    registry_enum_key_size = sizeof(registry_enum_key);

    if (RegOpenKeyEx(registry_key, registry_enum_key, 0, KEY_ALL_ACCESS, &registry_subkey) != ERROR_SUCCESS)
      continue;

    if (RegQueryValueEx(registry_subkey, "NetCfgInstanceId", NULL, NULL, registry_query_value, &registry_query_value_size) != ERROR_SUCCESS) {
      RegCloseKey(registry_subkey);
      continue;
    }

    if (!string_equals((const char *)registry_query_value, iface->adapter_name, true)) {
      RegCloseKey(registry_subkey);
      continue;
    }

    if (RegSetValueEx(registry_subkey, "NetworkAddress", 0, REG_SZ, (const BYTE *)stealth_mac_value, string_length(stealth_mac_value)+1) != ERROR_SUCCESS)
      break;

    RegCloseKey(registry_subkey);
    returned = 1;
    break;
  }

  RegCloseKey(registry_subkey);
  RegCloseKey(registry_key);

  return returned;
}

#elif defined __linux__ || defined __linux
#include <linux/if.h>
// #include <net/if.h>

#include <sys/ioctl.h>

#include <string.h>


static int
netstar_stealth_interface_set_status(struct netlink_interface *iface, netstar_stealth_interface_status_t status) {
  socket_t socket = SOCKET_INVALID;
  struct ifreq ifreq = {0};

  int returned = -1;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    goto _return;

  string_write(ifreq.ifr_name, iface->name, IFNAMSIZ);

  if (ioctl(socket, SIOCGIFFLAGS, &ifreq) == -1)
    goto _return;

  ifreq.ifr_flags = ((status == NETSTAR_STEALTH_INTERFACE_STATUS_UP) ? (ifreq.ifr_flags | IFF_UP) : (ifreq.ifr_flags & (~IFF_UP)));

  if (ioctl(socket, SIOCSIFFLAGS, &ifreq) == -1)
    goto _return;

  returned = 1;

_return:
  socket_close(socket);
  return returned;
}

static int
netstar_stealth_interface_macieee802(struct netlink_interface *iface, const network_macaddr_t *stealth_mac) {
  socket_t socket = SOCKET_INVALID;
  struct ifreq ifreq = {0};

  int returned = -1;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    goto _return;

  string_write(ifreq.ifr_name, iface->name, IFNAMSIZ);

  if (ioctl(socket, SIOCGIFFLAGS, &ifreq) == -1)
    goto _return;

  memcpy(&ifreq.ifr_hwaddr.sa_data, stealth_mac, NETWORK_MACADDR_SIZE);
  ifreq.ifr_hwaddr.sa_family = NETSTAR_ARP_HARDWARE_ETHERNET;

  if (ioctl(socket, SIOCSIFHWADDR, &ifreq) == -1)
    goto _return;

  returned = 1;

_return:
  socket_close(socket);
  return returned;
}

#elif (defined __APPLE__ && defined __MACH__) || (defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__ || defined _SYSTYPE_BSD)
// #include <net/ethernet.h>
#include <net/if_types.h>
#include <net/if.h>
#include <net/if_dl.h>

#include <sys/ioctl.h>

#include <string.h>


static int
netstar_stealth_interface_set_status(struct netlink_interface *iface, netstar_stealth_interface_status_t status) {
  socket_t socket = SOCKET_INVALID;
  struct ifreq ifreq = {0};

  int returned = -1;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    goto _return;

  string_write(ifreq.ifr_name, iface->name, IFNAMSIZ);

  if (ioctl(socket, SIOCGIFFLAGS, &ifreq) == -1)
    goto _return;

  ifreq.ifr_flags = ((status == NETSTAR_STEALTH_INTERFACE_STATUS_UP) ? (ifreq.ifr_flags | IFF_UP) : (ifreq.ifr_flags & (~IFF_UP)));

  if (ioctl(socket, SIOCSIFFLAGS, &ifreq) == -1)
    goto _return;

  returned = 1;

_return:
  socket_close(socket);
  return returned;
}

static int
netstar_stealth_interface_macieee802(struct netlink_interface *iface, const network_macaddr_t *stealth_mac) {
  struct sockaddr_dl datalink_addr = {0};
  struct ifreq ifreq = {0};

  socket_t socket = SOCKET_INVALID;

  int returned = -1;

  if ((socket = socket_new(AF_LOCAL, SOCK_DGRAM, IPPROTO_IP)) == SOCKET_INVALID)
    goto _return;

  string_write(ifreq.ifr_name, iface->name, IFNAMSIZ);

  memset(&datalink_addr, 0, sizeof(struct sockaddr_dl));
  datalink_addr.sdl_family = AF_LINK;
  datalink_addr.sdl_alen = NETWORK_MACADDR_SIZE;
  datalink_addr.sdl_nlen = 0;
  datalink_addr.sdl_len = sizeof(struct sockaddr_dl);

  memcpy(datalink_addr.sdl_data, stealth_mac, NETWORK_MACADDR_SIZE);

  ifreq.ifr_addr.sa_family = AF_LINK;
  ifreq.ifr_addr.sa_len = datalink_addr.sdl_alen;
  bcopy(LLADDR(&datalink_addr), &ifreq.ifr_addr.sa_data, datalink_addr.sdl_alen);

  if (ioctl(socket, SIOCSIFLLADDR, &ifreq) == -1)
    goto _return;

  returned = 1;

_return:
  socket_close(socket);
  return returned;
}

#else
#warning "netstar-stealth mode not implemented for this platform."

static int
netstar_stealth_interface_set_status(struct netlink_interface *iface, netstar_stealth_interface_status_t status) {
  return -1;
}

static int
netstar_stealth_interface_macieee802(struct netlink_interface *iface, const network_macaddr_t *stealth_mac) {
  return -1;
}
 
#endif

/*
static int
netstar_stealth_interface_initialized(void) {
  struct sockaddr_in server_addr = { .sin_family = AF_INET, .sin_addr.s_addr = NETWORK_IPADDR4_UBROADCAST, .sin_port = htons(17800) };  
  socket_t socket = SOCKET_INVALID;

  int status = -1, broadcast = 1;

  if ((socket = socket_new(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_INVALID)
    goto _return;

  if (socket_setoption(socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1)
    goto _return;

// network_ipaddr4_parse((network_ipaddr4_t *)&server_addr.sin_addr.s_addr, "8.8.8.8");

  while (socket_sendto(socket, "\xFF", 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    netstar_time_sleep(1000);

  status = 1;

_return:
  socket_close(socket);
  return status;
}
*/

static inline int
netstar_stealth_managed_interface_recreate_instance(netstar_t *netstar, const network_macaddr_t *stealth_mac) {
  struct netlink_interface *iface = netstar->managed.iface;
  netpacket_live_t *live = netstar->managed.live;

  netstar_managed_interface_set(netstar, iface->name);

  netlink_interface_free(iface);
  netpacket_live_close(live);

  if (network_macaddr_compare(&netstar->managed.iface->mac, stealth_mac) != 0)
    return -1;

  return 1;
}

int
netstar_stealth(netstar_t *netstar, const network_macaddr_t *stealth_mac) {
  netstar_log("\b \b[ stealth ] switching \"%s\" NIC address, entering stealth interface mode...\r\n", netstar->managed.iface->name);

  if (netstar_stealth_interface_set_status(netstar->managed.iface, NETSTAR_STEALTH_INTERFACE_STATUS_DOWN) == -1)
    return -1;

  netstar_time_sleep(netstar_time_secstomillisecs(4));

  if (netstar_stealth_interface_macieee802(netstar->managed.iface, stealth_mac) == -1) {
    netstar_critical("netstar-stealth: %s\r\n", system_error_reason(system_geterror()));
    return -1;
  }

  if (netstar_stealth_interface_set_status(netstar->managed.iface, NETSTAR_STEALTH_INTERFACE_STATUS_UP) == -1)
    return -1;

  netstar_time_sleep(netstar_time_secstomillisecs(4));

// if (netstar_stealth_interface_initialized() == -1)
//   return -1;

  if (netstar_stealth_managed_interface_recreate_instance(netstar, stealth_mac) == -1)
    return -1;

  netstar_log("\b \b[ stealth ] NIC address switched successfully; interface is now in stealth mode\r\n");

  return 1;
}
