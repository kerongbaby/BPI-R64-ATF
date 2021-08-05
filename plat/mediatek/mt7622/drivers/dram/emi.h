#ifndef EMI_H
#define EMI_H

struct emi_settings {
	int sub_version;	/* sub_version: 0x1 for new version */
	int type;		/* 0x0000 : Invalid
				   0x0001 : Discrete DDR1
				   0x0002 : Discrete LPDDR2
				   0x0003 : Discrete LPDDR3
				   0x0004 : Discrete PCDDR4
				   0x0005 : Discrete PCDDR5
				   0x0101 : MCP(NAND + DDR1)
				   0x0102 : MCP(NAND + LPDDR2)
				   0x0103 : MCP(NAND + LPDDR3)
				   0x0104 : MCP(NAND + PCDDR3)
				   0x0201 : MCP(eMMC + DDR1)
				   0x0202 : MCP(eMMC + LPDDR2)
				   0x0203 : MCP(eMMC + LPDDR3)
				   0x0204 : MCP(eMMC + PCDDR3)
				*/
	int id_length;		/* EMMC and NAND ID checking length */
	int fw_id_length;	/* FW ID checking length */
	char id[16];
	char fw_id[8];		/* To save fw id */
	int emi_cona_val;	/* 0x3000 */
	int emi_conh_val;
	int dramc_actim_val;	/* @0x4000 */
	int dramc_gddr3ctl1_val;/* @0x40F4 */
	int dramc_conf1_val;	/* @0x4004 */
	int dramc_ddr2ctl_val;	/* @0x407C */
	int dramc_test2_3_val;	/* @0x4044 */
	int dramc_conf2_val;	/* @0x4008 */
	int dramc_pd_ctrl_val;	/* @0x41DC */
	int dramc_actim1_val;	/* @0x41E8 */
	int dramc_misctl0_val;	/* @0x40FC */
	int dramc_actim05t_val;	/* @0x41F8 */
	int dramc_rkcfg_val;	/* @0x4110 */
	int dramc_test2_4_val;	/* @0x4048 */
	int dramc_rank_size[4];
	int reserved[10];

	union {
		struct {
			int ilpddr2_mode_reg_1;
			int ilpddr2_mode_reg_2;
			int ilpddr2_mode_reg_3;
			int ilpddr2_mode_reg_5;
			int ilpddr2_mode_reg_10;
			int ilpddr2_mode_reg_63;
		};
		struct {
			int iddr1_mode_reg;
			int iddr1_ext_mode_reg;
		};
		struct {
			int ipcddr3_mode_reg0;
			int ipcddr3_mode_reg1;
			int ipcddr3_mode_reg2;
			int ipcddr3_mode_reg3;
		};
		struct {
			int ilpddr3_mode_reg_1;
			int ilpddr3_mode_reg_2;
			int ilpddr3_mode_reg_3;
			int ilpddr3_mode_reg_5;
			int ilpddr3_mode_reg_10;
			int ilpddr3_mode_reg_63;
		};
	};
};

#if 0
struct emi_settings {
0x00 [   0] --	int sub_version;	/* sub_version: 0x1 for new version */
0x04 [   4] --	int type;		/* 0x0000 : Invalid
				   0x0001 : Discrete DDR1
				   0x0002 : Discrete LPDDR2
				   0x0003 : Discrete LPDDR3
				   0x0004 : Discrete PCDDR4
				   0x0005 : Discrete PCDDR5
				   0x0101 : MCP(NAND + DDR1)
				   0x0102 : MCP(NAND + LPDDR2)
				   0x0103 : MCP(NAND + LPDDR3)
				   0x0104 : MCP(NAND + PCDDR3)
				   0x0201 : MCP(eMMC + DDR1)
				   0x0202 : MCP(eMMC + LPDDR2)
				   0x0203 : MCP(eMMC + LPDDR3)
				   0x0204 : MCP(eMMC + PCDDR3)
				*/
0x08 [   8] --	int id_length;		/* EMMC and NAND ID checking length */
0x0c [  12] --	int fw_id_length;	/* FW ID checking length */
0x10 [  16] --	char id[16];
0x20 [  32] --	char fw_id[8];		/* To save fw id */
0x28 [  40] --	int emi_cona_val;	/* 0x3000 */
0x2c [  44] --	int emi_conh_val;
0x30 [  48] (400) --	int dramc_actim_val;	/* @0x4000 */
0x34 [  52] (404) --	int dramc_gddr3ctl1_val;/* @0x40F4 */
0x38 [  56] (408) --	int dramc_conf1_val;	/* @0x4004 */
0x3c [  60] (412) --	int dramc_ddr2ctl_val;	/* @0x407C */
0x40 [  64] (416) --	int dramc_test2_3_val;	/* @0x4044 */
0x44 [  68] (420) --	int dramc_conf2_val;	/* @0x4008 */
0x48 [  72] (424) --	int dramc_pd_ctrl_val;	/* @0x41DC */
0x4c [  76] (428) --	int dramc_actim1_val;	/* @0x41E8 */
0x50 [  80] (432) --	int dramc_misctl0_val;	/* @0x40FC */
0x54 [  84] (436) -- int dramc_actim05t_val;	/* @0x41F8 */
0x58 [  88] (440) --	int dramc_rkcfg_val;	/* @0x4110 */
0x5c [  92] (444) --	int dramc_test2_4_val;	/* @0x4048 */
0x60 [  96] (448) --	int dramc_rank_size[4];
0x70 [ 112] --	int reserved[10];

0x98 [ 152] --
	union {
		struct {
0x98 [ 152] --	int ilpddr2_mode_reg_1;
0x9c [ 156] --	int ilpddr2_mode_reg_2;
0xa0 [ 160] --	int ilpddr2_mode_reg_3;
0xa4 [ 164] --	int ilpddr2_mode_reg_5;
			int ilpddr2_mode_reg_10;
			int ilpddr2_mode_reg_63;
		};
		struct {
			int iddr1_mode_reg;
			int iddr1_ext_mode_reg;
		};
		struct {
			int ipcddr3_mode_reg0;
			int ipcddr3_mode_reg1;
			int ipcddr3_mode_reg2;
			int ipcddr3_mode_reg3;
		};
		struct {
			int ilpddr3_mode_reg_1;
			int ilpddr3_mode_reg_2;
			int ilpddr3_mode_reg_3;
			int ilpddr3_mode_reg_5;
			int ilpddr3_mode_reg_10;
			int ilpddr3_mode_reg_63;
		};
	};
};
#endif

int complex_mem_test(uintptr_t start, unsigned long len);
void mtk_set_emi_dram(void);
void mtk_mem_init(void);

#endif
