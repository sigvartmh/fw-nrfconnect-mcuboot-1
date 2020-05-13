#include <zephyr.h>
#include <hal/nrf_reset.h>
#define CONFIG_PCD_BUF_SIZE 1024
#include <dfu/pcd.h>
#include <pm_config.h>

int do_network_core_update(void *addr, void *src_addr, size_t len) {
	//struct pcd_cmd = (struct pcd_cmd *) addrfalse;
	nrf_reset_network_force_off(NRF_RESET, true);
	struct pcd_cmd *cmd = (struct pcd_cmd *)addr;
	cmd->magic = PCD_CMD_MAGIC_COPY;
	cmd->src = src_addr;
	cmd->len = len;
	cmd->offset = 0x10000;
	nrf_reset_network_force_off(NRF_RESET, false);
	int i = 0;
	while (cmd->magic == PCD_CMD_MAGIC_COPY){
		if(i == 0xffffff) {
			printk("cmd.magic: 0x%x\n", cmd->magic);
			printk("Waiting for magic to change.\n");
			i = 0;
		}
		i++;
	};

	if (cmd->magic == PCD_CMD_MAGIC_FAIL) {
		return -1;
	}

	return 0;
}
