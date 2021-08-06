#include <assert.h>
#include "emi.h"
#include "dramc.h"
#include "dramc_pi_api.h"
#include <lib/mmio.h>

#define EMI_SETTING_DEFAULT 2
static struct emi_settings emi_settings[3] = {
  {
    sub_version:            0,
    type:                   0x0003 /* Discrete LPDDR3 */,
    emi_cona_val:           0x20102017,
    emi_conh_val:           0x00000000,
    dramc_actim_val:        0xaafd478c,

    dramc_gddr3ctl1_val:    0x11000000, /* @0x40F4 */
    dramc_conf1_val:        0x00048603,	/* @0x4004 */
    dramc_ddr2ctl_val:      0x000063b1,	/* @0x407C */
    dramc_test2_3_val:      0xbfc70401,	/* @0x4044 */
    dramc_conf2_val:        0x030000a9,	/* @0x4008 */
    dramc_pd_ctrl_val:      0x15976442,	/* @0x41DC */
    dramc_actim1_val:       0x91001f59,	/* @0x41E8 */
    dramc_misctl0_val:      0x21000000,	/* @0x40FC */
    dramc_actim05t_val:     0x000025e1,	/* @0x41F8 */
    dramc_rkcfg_val:        0x002156c1,	/* @0x4110 */
    dramc_test2_4_val:      0x2801110d,	/* @0x4048 */
    dramc_rank_size:        {0x80000000},

    ilpddr2_mode_reg_1:     0x00830001,
    ilpddr2_mode_reg_2:     0x001c0002,
    ilpddr2_mode_reg_3:     0x00020003,
    ilpddr2_mode_reg_5:     0x00000006,
    ilpddr2_mode_reg_10:    0x00ff000a,
    ilpddr2_mode_reg_63:    0x0000003f,
  },
  {
    sub_version:          0,
    type:                 0x0004 /* Discrete LPDDR4 */,

    emi_cona_val:         0x00000010,
    emi_conh_val:         0x00000000,
    dramc_actim_val:      0x55d84408,

    dramc_gddr3ctl1_val:  0x11000000, /* @0x40F4 */
    dramc_conf1_val:      0xf07486e2,	/* @0x4004 */
    dramc_ddr2ctl_val:    0x85874207,	/* @0x407C */
    dramc_test2_3_val:    0xbfb00400,	/* @0x4044 */

    dramc_conf2_val:      0x10b0b070,	/* @0x4008 */
    dramc_pd_ctrl_val:    0xd3653840,	/* @0x41DC */
    dramc_actim1_val:     0x80ff0d50,	/* @0x41E8 */
    dramc_misctl0_val:    0x07010000,	/* @0x40FC */

    dramc_actim05t_val:   0x00001420,	/* @0x41F8 */
    dramc_rkcfg_val:      0x002156c1,	/* @0x4110 */
    dramc_test2_4_val:    0x2200110d,	/* @0x4048 */
    dramc_rank_size:      {0x08000000},

    ilpddr2_mode_reg_1:   0x00830001,
    ilpddr2_mode_reg_2:   0x001c0002,
    ilpddr2_mode_reg_3:   0x00020003,
    ilpddr2_mode_reg_5:   0x00000006,
    ilpddr2_mode_reg_10:  0x00ff000a,
    ilpddr2_mode_reg_63:  0x0000003f,
  },
  {
    sub_version:          0,
    type:                 0x0005 /* Discrete LPDDR5 */,

    emi_cona_val:         0x00003010,
    emi_conh_val:         0x00000000,
    dramc_actim_val:      0x448b07b6,
    dramc_gddr3ctl1_val:  0x11000000, /* @0x40F4 */
    dramc_conf1_val:      0xc0748680,	/* @0x4004 */
    dramc_ddr2ctl_val:    0x85872201,	/* @0x407C */
    dramc_test2_3_val:    0x9f3d0480,	/* @0x4044 */
    dramc_conf2_val:      0x00000060,	/* @0x4008 */
    dramc_pd_ctrl_val:    0x15603840,	/* @0x41DC */
    dramc_actim1_val:     0x80000050,	/* @0x41E8 */
    dramc_misctl0_val:    0x07000000,	/* @0x40FC */
    dramc_actim05t_val:   0x040000d1,	/* @0x41F8 */
    dramc_rkcfg_val:      0x00010400,	/* @0x4110 */
    dramc_test2_4_val:    0x1e00d10d,	/* @0x4048 */
    dramc_rank_size:      {0x20000000},

    ilpddr2_mode_reg_1:   0x00001d71,
    ilpddr2_mode_reg_2:   0x00002000,
    ilpddr2_mode_reg_3:   0x00004018,
    ilpddr2_mode_reg_5:   0x00006000,
    ilpddr2_mode_reg_10:  0x00000024,
    ilpddr2_mode_reg_63:  0x00000000,
  },
};

static struct dramc_ctx_t dram_ctx_pcddr2 = {
	channel:              CHANNEL_A,
	dram_type:            TYPE_PCDDR2,
	package:              PACKAGE_SBS,
	data_width:           DATA_WIDTH_16BIT,
	test2_1:              0x55000000,
	test2_2:              0xaa000400,
  test_pattern:         TEST_XTALK_PATTERN,
	frequency:            0x0215,
	frequency_low:        0x0215,
	fglow_freq_write_en:  0x0,
	ssc_en:               0x0,
	en_4bitMux:           0x0,
};
static struct dramc_ctx_t dram_ctx_pcddr3 = {
	channel:              CHANNEL_A,
	dram_type:            TYPE_PCDDR3,
	package:              PACKAGE_SBS,
	data_width:           DATA_WIDTH_16BIT,
	test2_1:              0x55000000,
	test2_2:              0xaa000400,
	test_pattern:         TEST_XTALK_PATTERN,
	frequency:            0x0215,
	frequency_low:        0x0215,
	fglow_freq_write_en:  0x0,
	ssc_en:               0x0,
	en_4bitMux:           0x0,
};

void mtk_set_emi_dram_wrap(void) {
  struct dramc_ctx_t *dram_ctx_pcddr = NULL;
  unsigned int type = emi_settings[EMI_SETTING_DEFAULT].type & 0x0f;
  if(type == 0x04)
    dram_ctx_pcddr = &dram_ctx_pcddr2;
  else if(type == 0x05)
    dram_ctx_pcddr = &dram_ctx_pcddr3;
  else {
    NOTICE("unsupport type.\n");
    return;
  }

  mmio_write_32(0x10203000 + 0 /* EMI_BASE + EMI_CONA */, emi_settings[EMI_SETTING_DEFAULT].emi_cona_val);
  mmio_write_32(0x10203000 + 96 /* EMI_BASE + EMI_CONM */, 0x6b8);
// #define EMI_CONF	(EMI_BASE + 0x028)	/* Address scramble setting */
  mmio_write_32(0x10203000 + 0x028 /* EMI_BASE + EMI_CONF */, 0x00421000);
  if(dram_ctx_pcddr->dram_type == 0x5) {
  DRAMC_WRITE_REG(0x110, 0x3a64592);
  DRAMC_WRITE_REG(0x640, 0x10);
  DRAMC_WRITE_REG(0x430, 0x10ff10ff);
  DRAMC_WRITE_REG(0x434, 0xffffffff);
  DRAMC_WRITE_REG(0x438, 0xffffffff);
  DRAMC_WRITE_REG(0x43c, 0x1f);
  DRAMC_WRITE_REG(0x400, 0x11111111);
  DRAMC_WRITE_REG(0x440, 0x0);
  DRAMC_WRITE_REG(0x444, 0x0);
  DRAMC_WRITE_REG(0x448, 0x0);
  DRAMC_WRITE_REG(0x44c, 0x0);
  DRAMC_WRITE_REG(0x445, 0x0);
  DRAMC_WRITE_REG(0x404, 0x01111111);
  DRAMC_WRITE_REG(0x408, 0x11111111);
  DRAMC_WRITE_REG(0x40c, 0x11111111);
  DRAMC_WRITE_REG(0x418, 0x00000216);
  DRAMC_WRITE_REG(0x41c, 0x33334444);
  DRAMC_WRITE_REG(0x420, 0x33334444);
  DRAMC_WRITE_REG(0x424, 0x33334444);
  DRAMC_WRITE_REG(0x428, 0xffff5555);
  DRAMC_WRITE_REG(0x42c, 0x00ff0055);
  DRAMC_WRITE_REG(0x410, 0x04855555);
  DRAMC_WRITE_REG(0x1f8, emi_settings[EMI_SETTING_DEFAULT].dramc_actim05t_val);
  DRAMC_WRITE_REG(0x1e0, 0x0);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x08c, 0xe00000 /* w26_0xe00000 */);
  DRAMC_WRITE_REG(0x07c, 0x81);
  DRAMC_WRITE_REG(0x0f4, 0x1000000);
  DRAMC_WRITE_REG(0x110, 0x3a64592);
  DRAMC_WRITE_REG(0x028, 0xf1200f01);
  DRAMC_WRITE_REG(0x1e8, 0x80ff1250);
  DRAMC_WRITE_REG(0x008, 0x08a0ae00);
  DRAMC_WRITE_REG(0x1ec, 0x8120000);
  DRAMC_WRITE_REG(0x080, 0xe000e0);
  DRAMC_WRITE_REG(0x0d8, 0x80000500);
  DRAMC_WRITE_REG(0x0f0, 0x82000000);
  DRAMC_WRITE_REG(0x0f8, 0xedcb2000);
  DRAMC_WRITE_REG(0x5c0, 0x2156ff83);
  DRAMC_WRITE_REG(0x5c4, 0x5083001e);
  DRAMC_WRITE_REG(0x5c8, 0x1010);
  DRAMC_WRITE_REG(0x5cc, 0x0);
  DRAMC_WRITE_REG(0x640, 0x10);
  DRAMC_WRITE_REG(0x1e0, 0x0);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x0f4, 0x11000000);
  DRAMC_WRITE_REG(0x698, 0x4000);
  DRAMC_WRITE_REG(0x63c, 0x6);
  DRAMC_WRITE_REG(0x0fc, emi_settings[EMI_SETTING_DEFAULT].dramc_misctl0_val);
  DRAMC_WRITE_REG(0x048, emi_settings[EMI_SETTING_DEFAULT].dramc_test2_4_val);
  DRAMC_WRITE_REG(0x118, 0x84);
  DRAMC_WRITE_REG(0x0e4, 0x020000b3);
  udelay(500);
  DRAMC_WRITE_REG(0x138, 0x8415);
  DRAMC_WRITE_REG(0x08c, 0xe00000);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x098, 0x1f1f1f1f);
  DRAMC_WRITE_REG(0x094, 0x1f1f1f1f);
  DRAMC_WRITE_REG(0x840, 0x3f3f3f3f);
  DRAMC_WRITE_REG(0x844, 0x3f3f3f3f);
  DRAMC_WRITE_REG(0x0e0, 0x04800800);
  DRAMC_WRITE_REG(0x0dc, 0x00800800);
  DRAMC_WRITE_REG(0x054, 0x900);
  DRAMC_WRITE_REG(0x1c4, 0x50000);
  udelay(30);
  DRAMC_WRITE_REG(0x0f4, emi_settings[EMI_SETTING_DEFAULT].dramc_gddr3ctl1_val);
  DRAMC_WRITE_REG(0x130, 0x30000000);
  udelay(1000);
  DRAMC_WRITE_REG(0x004, emi_settings[EMI_SETTING_DEFAULT].dramc_conf1_val);
  DRAMC_WRITE_REG(0x124, 0xc006001a);
  DRAMC_WRITE_REG(0x1c0, 0x400000);
  DRAMC_WRITE_REG(0x004, emi_settings[EMI_SETTING_DEFAULT].dramc_ddr2ctl_val);
  DRAMC_WRITE_REG(0x080, 0x00ee0ae0);
  DRAMC_WRITE_REG(0x110, 0x3a64592);
  DRAMC_WRITE_REG(0x158, 0xff0);
  DRAMC_WRITE_REG(0x1e0, 0x80b7);
  DRAMC_WRITE_REG(0x0e4, 0x80b7);
  DRAMC_WRITE_REG(0x0b8, 0xaa22aa22);
  DRAMC_WRITE_REG(0x0bc, 0xae22aa22);
  DRAMC_WRITE_REG(0x008, 0x10b0b070);
  udelay(1000);
  DRAMC_WRITE_REG(0x054, (DRAMC_READ_REG(0x054) & 0xfffffffe));
  DRAMC_WRITE_REG(0x0d8, (DRAMC_READ_REG(0x0d8) & 0xff7fffff));
  DRAMC_WRITE_REG(0x07c, (DRAMC_READ_REG(0x07c) | 0xc));
  udelay(2000);
  DRAMC_WRITE_REG(0x088, emi_settings[EMI_SETTING_DEFAULT].ilpddr2_mode_reg_3); // ??
  DRAMC_WRITE_REG(0x1e4, 0x1);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  DRAMC_WRITE_REG(0x088, emi_settings[EMI_SETTING_DEFAULT].ilpddr2_mode_reg_5); // ??
  DRAMC_WRITE_REG(0x1e4, 0x1);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  DRAMC_WRITE_REG(0x088, emi_settings[EMI_SETTING_DEFAULT].ilpddr2_mode_reg_2);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  DRAMC_WRITE_REG(0x088, emi_settings[EMI_SETTING_DEFAULT].ilpddr2_mode_reg_1);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  DRAMC_WRITE_REG(0x088, 0x400);
  DRAMC_WRITE_REG(0x1e4, 0x10);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1000);
  DRAMC_WRITE_REG(0x1e4, 0x1100);
  DRAMC_WRITE_REG(0x1e4, 0xa3);
  DRAMC_WRITE_REG(0x088, 0xfbff);
  DRAMC_WRITE_REG(0x1e4, 0x20);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  DRAMC_WRITE_REG(0x0, emi_settings[EMI_SETTING_DEFAULT].dramc_actim_val);
  DRAMC_WRITE_REG(0x1f8, emi_settings[EMI_SETTING_DEFAULT].dramc_actim05t_val);
  DRAMC_WRITE_REG(0x044, emi_settings[EMI_SETTING_DEFAULT].dramc_test2_3_val);
  DRAMC_WRITE_REG(0x008, emi_settings[EMI_SETTING_DEFAULT].dramc_conf2_val);
  DRAMC_WRITE_REG(0x010, 0x0);
  DRAMC_WRITE_REG(0x100, 0x8110);
  DRAMC_WRITE_REG(0x10c, 0x12121212);
  DRAMC_WRITE_REG(0x1dc, emi_settings[EMI_SETTING_DEFAULT].dramc_pd_ctrl_val);
  DRAMC_WRITE_REG(0x1e4, 0x020044b3);
  DRAMC_WRITE_REG(0x110, 0x03a64580);
  DRAMC_WRITE_REG(0x008, 0x10a0b070);
  DRAMC_WRITE_REG(0x008, (DRAMC_READ_REG(0x008) & 0xffffff00));
  DRAMC_WRITE_REG(0x110, (DRAMC_READ_REG(0x110) & 0xffffffef));
  dle_factor_handler(dram_ctx_pcddr, 0x10 /* uint8_t curr_val */);
  DRAMC_WRITE_REG(0x018, 0x1616);
  DRAMC_WRITE_REG(0x01c, 0x20202020);
  DRAMC_WRITE_REG(0x210, 0x0);
  DRAMC_WRITE_REG(0x214, 0x0);
  DRAMC_WRITE_REG(0x218, 0x0);
  DRAMC_WRITE_REG(0x21c, 0x0);
  DRAMC_WRITE_REG(0x220, 0x0);
  DRAMC_WRITE_REG(0x224, 0x0);
  DRAMC_WRITE_REG(0x228, 0x0);
  DRAMC_WRITE_REG(0x22c, 0x0);
  DRAMC_WRITE_REG(0x014, 0x0);
  DRAMC_WRITE_REG(0x010, 0x0);
  DRAMC_WRITE_REG(0x200, 0x0);
  DRAMC_WRITE_REG(0x204, 0x0);
  DRAMC_WRITE_REG(0x208, 0x0);
  DRAMC_WRITE_REG(0x20c, 0x0);
} else if(dram_ctx_pcddr->dram_type == 0x4) {
  DRAMC_WRITE_REG(0x1f8, emi_settings[EMI_SETTING_DEFAULT].dramc_actim05t_val);
  DRAMC_WRITE_REG(0x1e0, 0x0);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x0f8, 0xedcb2000);
  DRAMC_WRITE_REG(0x08c, 0xe00000);
  DRAMC_WRITE_REG(0x640, 0x10);
  DRAMC_WRITE_REG(0x1e0, 0x0);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x0f4, 0x11000000);
  DRAMC_WRITE_REG(0x0f4, 0x4000);
  DRAMC_WRITE_REG(0x63c, 0x6);
  DRAMC_WRITE_REG(0x5c0, 0xff83);
  DRAMC_WRITE_REG(0x5c8, 0x1010);
  DRAMC_WRITE_REG(0x5cc, 0x0);
  DRAMC_WRITE_REG(0x1e4, 0x40000000);
  DRAMC_WRITE_REG(0x000, emi_settings[EMI_SETTING_DEFAULT].dramc_actim_val);
  DRAMC_WRITE_REG(0x004, 0xf07402e2);
  DRAMC_WRITE_REG(0x008, 0x10b0b050);
  DRAMC_WRITE_REG(0x044, emi_settings[EMI_SETTING_DEFAULT].dramc_test2_3_val);
  DRAMC_WRITE_REG(0x07c, emi_settings[EMI_SETTING_DEFAULT].dramc_ddr2ctl_val);
  DRAMC_WRITE_REG(0x0fc, emi_settings[EMI_SETTING_DEFAULT].dramc_misctl0_val);
  DRAMC_WRITE_REG(0x1dc, emi_settings[EMI_SETTING_DEFAULT].dramc_pd_ctrl_val);
  DRAMC_WRITE_REG(0x1e8, emi_settings[EMI_SETTING_DEFAULT].dramc_actim1_val);
  DRAMC_WRITE_REG(0x1f8, emi_settings[EMI_SETTING_DEFAULT].dramc_actim05t_val);
  DRAMC_WRITE_REG(0x048, emi_settings[EMI_SETTING_DEFAULT].dramc_test2_4_val);
  DRAMC_WRITE_REG(0x110, 0x3a64580);
  DRAMC_WRITE_REG(0x430, 0x10ff10ff);
  DRAMC_WRITE_REG(0x434, 0xffffffff);
  DRAMC_WRITE_REG(0x438, 0xffffffff);
  DRAMC_WRITE_REG(0x43c, 0x1f);
  DRAMC_WRITE_REG(0x400, 0x11111111);
  DRAMC_WRITE_REG(0x400, 0x01010111);
  DRAMC_WRITE_REG(0x408, 0x11111111);
  DRAMC_WRITE_REG(0x40c, 0x11111111);
  DRAMC_WRITE_REG(0x410, 0x07555555);
  DRAMC_WRITE_REG(0x414, 0x55555555);
  DRAMC_WRITE_REG(0x418, 0x217);
  DRAMC_WRITE_REG(0x41c, 0x22222222);
  DRAMC_WRITE_REG(0x420, 0x22222222);
  DRAMC_WRITE_REG(0x424, 0x22222222);
  DRAMC_WRITE_REG(0x428, 0x5555ffff);
  DRAMC_WRITE_REG(0x42c, 0x005500ff);
  DRAMC_WRITE_REG(0x124, 0xea000011);
  DRAMC_WRITE_REG(0x124, 0xc0080011);
  DRAMC_WRITE_REG(0x138, 0x10);
  DRAMC_WRITE_REG(0x098, 0x1f1f1f1f);
  DRAMC_WRITE_REG(0x094, 0x40404040);
  DRAMC_WRITE_REG(0x840, 0x3f3f3f3f);
  DRAMC_WRITE_REG(0x844, 0x3f3f3f3f);
  DRAMC_WRITE_REG(0x1e0, 0x0);
  DRAMC_WRITE_REG(0x440, 0x0);
  DRAMC_WRITE_REG(0x444, 0x0);
  DRAMC_WRITE_REG(0x448, 0x0);
  DRAMC_WRITE_REG(0x44c, 0x0);
  DRAMC_WRITE_REG(0x450, 0x0);
  DRAMC_WRITE_REG(0x23c, 0x0);
  DRAMC_WRITE_REG(0x1c0, 0xc8b8);
  DRAMC_WRITE_REG(0x138, 0x8417);
  DRAMC_WRITE_REG(0x054, 0x900);
  DRAMC_WRITE_REG(0x028, 0xd32cff01);
  DRAMC_WRITE_REG(0x1c4, 0x70000);
  DRAMC_WRITE_REG(0x118, 0x83);
  DRAMC_WRITE_REG(0x080, 0x00ec08e9);
  DRAMC_WRITE_REG(0x0e0, 0x13200200);
  DRAMC_WRITE_REG(0x0dc, 0x00800800);
  udelay(0x1);
  DRAMC_WRITE_REG(0x1c4, 0x50000);
  DRAMC_WRITE_REG(0x138, 0x8415);
  DRAMC_WRITE_REG(0x0e4, 0x8011);
  DRAMC_WRITE_REG(0x008, 0x10b0b000);
  DRAMC_WRITE_REG(0x110, 0x03a64592);
  DRAMC_WRITE_REG(0x004, 0xf07486e2);
  DRAMC_WRITE_REG(0x0e4, 0x17);
  udelay(1);
  DRAMC_WRITE_REG(0x1e4, dram_ctx_pcddr->dram_type);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0x4000);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0x6000);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0x2040);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0xb63);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x1e4, dram_ctx_pcddr->dram_type);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x1e4, 0x8);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x1e4, 0x8);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0xa63);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0x23c0);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0x2000);
  DRAMC_WRITE_REG(0x1e4, 0x1);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x088, 0xffff);
  DRAMC_WRITE_REG(0x1e4, 0x20);
  DRAMC_WRITE_REG(0x1e4, 0x0);
  udelay(1);
  DRAMC_WRITE_REG(0x0e4, 0x93);
  DRAMC_WRITE_REG(0x110, 0x3a64582);
  DRAMC_WRITE_REG(0x008, 0x10a0b070);
  DRAMC_WRITE_REG(0x07c, 0x8287420f);
  DRAMC_WRITE_REG(0x054, (DRAMC_READ_REG(0x054) & 0xfffffffe));
  DRAMC_WRITE_REG(0x0d8, (DRAMC_READ_REG(0x0d8) & 0xff7fffff));
  DRAMC_WRITE_REG(0x07c, (DRAMC_READ_REG(0x07c) | 0xc));
  DRAMC_WRITE_REG(0x018, 0x1f1f);
  DRAMC_WRITE_REG(0x648, (DRAMC_READ_REG(0x648) | 0x20000));
} else {
  NOTICE("type unsupport?");
  return;
}

  do_calib(dram_ctx_pcddr, &emi_settings[2]);
  mmio_write_32(0x10203000 + 0 /* EMI_BASE + EMI_CONA */, 0x3020);
  if( complex_mem_test(0x40000000, 0x1000) == 0) {
    // 0x60 [  96] (448) --	int dramc_rank_size[4];
    emi_settings[EMI_SETTING_DEFAULT].dramc_rank_size[0] = 0x80000000;
  } else {
    emi_settings[EMI_SETTING_DEFAULT].dramc_rank_size[0] = 0x40000000;
    mmio_write_32(0x10203000 , 0x3010);
    mmio_write_32(0x10214044 , ((mmio_read_32(0x10214044) & 0xfff0ffff) | 0xd0000));
    mmio_write_32(0x102141e8 , ((mmio_read_32(0x102141e8) & 0xffffff0f) | 0x50));
  }
  return;
}

