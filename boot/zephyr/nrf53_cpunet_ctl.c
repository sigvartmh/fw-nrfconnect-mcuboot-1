/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <hal/nrf_reset.h>
#include <hal/nrf_spu.h>
#include <dfu/pcd.h>
#include <pm_config.h>
#include "bootutil/bootutil_log.h"
MCUBOOT_LOG_MODULE_DECLARE(mcuboot);

#define NET_CORE_APP_OFFSET PM_CPUNET_B0N_SIZE

int do_network_core_update(void *src_addr, size_t len)
{
	int ret;
	struct pcd_cmd *cmd;

	nrf_spu_extdomain_set(NRF_SPU, 0, true, false);

	/* Ensure that the network core is turned off */
	nrf_reset_network_force_off(NRF_RESET, true);

	BOOT_LOG_INF("Writing cmd to addr: 0x%x", PCD_CMD_ADDRESS);
	cmd = pcd_cmd_write((void *)PCD_CMD_ADDRESS, src_addr, len,
			    NET_CORE_APP_OFFSET);
	if (cmd == NULL) {
		BOOT_LOG_INF("Error while writing PCD cmd");
		return -1;
	}

	nrf_reset_network_force_off(NRF_RESET, false);
	BOOT_LOG_INF("Turned on network core");

	do {
		ret = pcd_status(cmd);
	} while(ret == 0);

	if (ret < 0) {
		BOOT_LOG_ERR("Network core update failed");
		return ret;
	}

	nrf_reset_network_force_off(NRF_RESET, true);
	BOOT_LOG_INF("Turned off network core");

	return 0;
}

void lock_ipc_ram_with_spu(){
	nrf_spu_ramregion_set(NRF_SPU,
			      APP_CORE_SRAM_SIZE/CONFIG_NRF_SPU_RAM_REGION_SIZE,
			      true, NRF_SPU_MEM_PERM_READ, true);
}
