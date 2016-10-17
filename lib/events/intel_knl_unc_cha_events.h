/*
 * Copyright (c) 2016 Intel Corp. All rights reserved
 * Contributed by Peinan Zhang <peinan.zhang@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of libpfm, a performance monitoring support library for
 * applications on Linux.
 *
 * PMU: knl_unc_cha (Intel Knights Landing CHA uncore PMU)
 */

static const intel_x86_umask_t knl_unc_cha_llc_lookup[]={
   { .uname  = "DATA_READ",
     .udesc  = "Data read requests",
     .ucode  = 0x0300,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "WRITE",
     .udesc  = "Write requests. Includes all write transactions (cached, uncached)",
     .ucode  = 0x0500,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "REMOTE_SNOOP",
     .udesc  = "External snoop request",
     .ucode  = 0x0900,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "ANY",
     .udesc  = "Any request",
     .ucode  = 0x1100,
     .uflags = INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
};

static const intel_x86_umask_t knl_unc_cha_llc_victims[]={
   { .uname  = "M_STATE",
     .udesc  = "Lines in M state",
     .ucode  = 0x0100,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "E_STATE",
     .udesc  = "Lines in E state",
     .ucode  = 0x0200,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "S_STATE",
     .udesc  = "Lines in S state",
     .ucode  = 0x0400,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "F_STATE",
     .udesc  = "Lines in F state",
     .ucode  = 0x0800,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "LOCAL",
     .udesc  = "Victimized Lines matching the NID filter.",
     .ucode  = 0x2000,
     .uflags = INTEL_X86_NCOMBO | INTEL_X86_DFL,
   },
   { .uname  = "REMOTE",
     .udesc  = "Victimized Lines does not matching the NID.",
     .ucode  = 0x8000,
     .uflags = INTEL_X86_NCOMBO,
   },
};


static const intel_x86_umask_t knl_unc_cha_ingress_int_starved[]={
   { .uname  = "IRQ",
     .udesc  = "Internal starved with IRQ.",
     .ucode  = 0x0100,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "IPQ",
     .udesc  = "Internal starved with IPQ.",
     .ucode  = 0x0400,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "ISMQ",
     .udesc  = "Internal starved with ISMQ.",
     .ucode  = 0x0800,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "PRQ",
     .udesc  = "Internal starved with PRQ.",
     .ucode  = 0x1000,
     .uflags = INTEL_X86_NCOMBO,
   },
};

static const intel_x86_umask_t knl_unc_cha_ingress_ext[]={
   { .uname  = "IRQ",
     .udesc  = "IRQ",
     .ucode  = 0x0100,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "IRQ_REJ",
     .udesc  = "IRQ rejected",
     .ucode  = 0x0200,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "IPQ",
     .udesc  = "IPQ",
     .ucode  = 0x0400,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "PRQ",
     .udesc  = "PRQ",
     .ucode  = 0x1000,
     .uflags = INTEL_X86_NCOMBO,
   },
   { .uname  = "PRQ_REJ",
     .udesc  = "PRQ rejected",
     .ucode  = 0x2000,
     .uflags = INTEL_X86_NCOMBO,
   },
};


static const intel_x86_umask_t knl_unc_cha_ingress_entry_reject_q0[]={
  { .uname  = "AD_REQ_VN0",
    .udesc  = "AD Request",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AD_RSP_VN0",
    .udesc  = "AD Response",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_RSP_VN0",
    .udesc  = "BL Response",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_WB_VN0",
    .udesc  = "BL WB",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_NCB_VN0",
    .udesc  = "BL NCB",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_NCS_VN0",
    .udesc  = "BL NCS",
    .ucode  = 0x2000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK_NON_UPI",
    .udesc  = "AK non upi",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV_NON_UPI",
    .udesc  = "IV non upi",
    .ucode  = 0x8000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ingress_entry_reject_q1[]={
  { .uname  = "ANY_REJECT",
    .udesc  = "Any reject from request queue0",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO | INTEL_X86_DFL,
  },
  { .uname  = "SF_VICTIM",
    .udesc  = "SF victim",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "SF_WAY",
    .udesc  = "SF way",
    .ucode  = 0x2000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "ALLOW_SNP",
    .udesc  = "allow snoop",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "PA_MATCH",
    .udesc  = "PA match",
    .ucode  = 0x8000,
    .uflags = INTEL_X86_NCOMBO,
  },
};


static const intel_x86_umask_t knl_unc_cha_tor_subevent[]={
  { .uname  = "IRQ",
    .udesc  = " -IRQ.",
    .ucode  = 0x3100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "EVICT",
    .udesc  = " -SF/LLC Evictions.",
    .ucode  = 0x3200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "PRQ",
    .udesc  = " -PRQ.",
    .ucode  = 0x3400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IPQ",
    .udesc  = " -IPQ.",
    .ucode  = 0x3800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "HIT",
    .udesc  = " -Hit (Not a Miss).",
    .ucode  = 0x1f00,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "MISS",
    .udesc  = " -Miss.",
    .ucode  = 0x2f00,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IRQ_HIT",
    .udesc  = " -IRQ HIT.",
    .ucode  = 0x1100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IRQ_MISS",
    .udesc  = " -IRQ MISS.",
    .ucode  = 0x2100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "PRQ_HIT",
    .udesc  = " -PRQ HIT.",
    .ucode  = 0x1400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "PRQ_MISS",
    .udesc  = " -PRQ MISS.",
    .ucode  = 0x2400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IPQ_HIT",
    .udesc  = " -IPQ HIT",
    .ucode  = 0x1800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IPQ_MISS",
    .udesc  = " -IPQ MISS",
    .ucode  = 0x2800,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_misc[]={
  { .uname  = "RSPI_WAS_FSE",
    .udesc  = "Silent Snoop Eviction",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "WC_ALIASING",
    .udesc  = "Write Combining Aliasing.",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "RFO_HIT_S",
    .udesc  = "Counts the number of times that an RFO hits in S state.",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "CV0_PREF_VIC",
    .udesc  = "CV0 Prefetch Victim.",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "CV0_PREF_MISS",
    .udesc  = "CV0 Prefetch Miss.",
    .ucode  = 0x2000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_tgr_ext[]={
  { .uname  = "TGR0",
    .udesc  = "for Transgress 0",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR1",
    .udesc  = "for Transgress 1",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR2",
    .udesc  = "for Transgress 2",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR3",
    .udesc  = "for Transgress 3",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR4",
    .udesc  = "for Transgress 4",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR5",
    .udesc  = "for Transgress 5",
    .ucode  = 0x2000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR6",
    .udesc  = "for Transgress 6",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "TGR7",
    .udesc  = "for Transgress 7",
    .ucode  = 0x8000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_tgr_ext1[]={
  { .uname  = "TGR8",
    .udesc  = "for Transgress 8",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "ANY_OF_TGR0_THRU_TGR7",
    .udesc  = "for Transgress 0-7",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_type_agent[]={
  { .uname  = "AD_AG0",
    .udesc  = "AD - Agent 0",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK_AG0",
    .udesc  = "AK - Agent 0",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_AG0",
    .udesc  = "BL - Agent 0",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV_AG0",
    .udesc  = "IV - Agent 0",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AD_AG1",
    .udesc  = "AD - Agent 1",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK_AG1",
    .udesc  = "AK - Agent 1",
    .ucode  = 0x2000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_AG1",
    .udesc  = "BL - Agent 1",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_type[]={
  { .uname  = "AD",
    .udesc  = " - AD ring",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK",
    .udesc  = " - AK ring",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL",
    .udesc  = " - BL ring",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV",
    .udesc  = " - IV ring",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_dire_ext[]={
  { .uname  = "VERT",
    .udesc  = " - vertical",
    .ucode  = 0x0000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "HORZ",
    .udesc  = " - horizontal",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_use_vert[]={
  { .uname  = "UP_EVEN",
    .udesc  = "UP_EVEN",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "UP_ODD",
    .udesc  = "UP_ODD",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "DN_EVEN",
    .udesc  = "DN_EVEN",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "DN_ODD",
    .udesc  = "DN_ODD",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_use_hori[]={
  { .uname  = "LEFT_EVEN",
    .udesc  = "LEFT_EVEN",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "LEFT_ODD",
    .udesc  = "LEFT_ODD",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "RIGHT_EVEN",
    .udesc  = "RIGHT_EVEN",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "RIGHT_ODD",
    .udesc  = "RIGHT_ODD",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_use_updn[]={
  { .uname  = "UP",
    .udesc  = "up",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "DN",
    .udesc  = "down",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_ring_use_lfrt[]={
  { .uname  = "LEFT",
    .udesc  = "left",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "RIGHT",
    .udesc  = "right",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_iv_snp[]={
  { .uname  = "IV_SNP_GO_UP",
    .udesc  = "IV_SNP_GO_UP",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV_SNP_GO_DN",
    .udesc  = "IV_SNP_GO_DN",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_cms_ext[]={
  { .uname  = "AD_BNC",
    .udesc  = "AD_BNC",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK_BNC",
    .udesc  = "AK_BNC",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_BNC",
    .udesc  = "BL_BNC",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV_BNC",
    .udesc  = "IV_BNC",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AD_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_cms_crd_starved[]={
  { .uname  = "AD_BNC",
    .udesc  = "AD_BNC",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AK_BNC",
    .udesc  = "AK_BNC",
    .ucode  = 0x0200,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_BNC",
    .udesc  = "BL_BNC",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IV_BNC",
    .udesc  = "IV_BNC",
    .ucode  = 0x0800,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AD_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "IVF",
    .udesc  = "IVF",
    .ucode  = 0x8000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_umask_t knl_unc_cha_cms_busy_starved[]={
  { .uname  = "AD_BNC",
    .udesc  = "AD_BNC",
    .ucode  = 0x0100,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_BNC",
    .udesc  = "BL_BNC",
    .ucode  = 0x0400,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "AD_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x1000,
    .uflags = INTEL_X86_NCOMBO,
  },
  { .uname  = "BL_CRD",
    .udesc  = "AD_CRD",
    .ucode  = 0x4000,
    .uflags = INTEL_X86_NCOMBO,
  },
};

static const intel_x86_entry_t intel_knl_unc_cha_pe[]={
  { .name    = "UNC_H_U_CLOCKTICKS",
    .desc    = "Uncore clockticks",
    .modmsk = 0x0,
    .cntmsk  = 0xf,
    .code    = 0x00,
    .flags = INTEL_X86_FIXED,
  },
  { .name    = "UNC_H_INGRESS_OCCUPANCY",
    .desc    = "Ingress Occupancy. Ingress Occupancy. Counts number of entries in the specified Ingress queue in each cycle",
    .cntmsk  = 0xf,
    .code    = 0x11,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_ext),
    .umasks  = knl_unc_cha_ingress_ext,
  },
  { .name    = "UNC_H_INGRESS_INSERTS",
    .desc    = "Ingress Allocations. Counts number of allocations per cycle into the specified Ingress queue",
    .cntmsk  = 0xf,
    .code    = 0x13,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_ext),
    .umasks  = knl_unc_cha_ingress_ext,
  },
  { .name    = "UNC_H_INGRESS_INT_STARVED",
    .desc    = "Cycles Internal Starvation",
    .cntmsk  = 0xf,
    .code    = 0x14,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_int_starved),
    .umasks  = knl_unc_cha_ingress_int_starved,
  },
  { .name    = "UNC_H_INGRESS_RETRY_IRQ0_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x18,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_IRQ01_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x19,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q1),
    .umasks  = knl_unc_cha_ingress_entry_reject_q1,
  },
  { .name    = "UNC_H_INGRESS_RETRY_PRQ0_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x20,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_PRQ1_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x21,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q1),
    .umasks  = knl_unc_cha_ingress_entry_reject_q1,
  },
  { .name    = "UNC_H_INGRESS_RETRY_IPQ0_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x22,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_IPQ1_REJECT",
    .desc    = "Ingress Request Queue Rejects",
    .cntmsk  = 0xf,
    .code    = 0x23,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q1),
    .umasks  = knl_unc_cha_ingress_entry_reject_q1,
  },
  { .name    = "UNC_H_INGRESS_RETRY_ISMQ0_REJECT",
    .desc    = "ISMQ Rejects",
    .cntmsk  = 0xf,
    .code    = 0x24,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_REQ_Q0_RETRY",
    .desc    = "REQUESTQ includes:  IRQ, PRQ, IPQ, RRQ, WBQ (everything except for ISMQ)",
    .cntmsk  = 0xf,
    .code    = 0x2a,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_REQ_Q1_RETRY",
    .desc    = "REQUESTQ includes:  IRQ, PRQ, IPQ, RRQ, WBQ (everything except for ISMQ)",
    .cntmsk  = 0xf,
    .code    = 0x2b,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q1),
    .umasks  = knl_unc_cha_ingress_entry_reject_q1,
  },
  { .name    = "UNC_H_INGRESS_RETRY_ISMQ0_RETRY",
    .desc    = "ISMQ retries",
    .cntmsk  = 0xf,
    .code    = 0x2c,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_OTHER0_RETRY",
    .desc    = "Other Queue Retries",
    .cntmsk  = 0xf,
    .code    = 0x2e,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q0),
    .umasks  = knl_unc_cha_ingress_entry_reject_q0,
  },
  { .name    = "UNC_H_INGRESS_RETRY_OTHER1_RETRY",
    .desc    = "Other Queue Retries",
    .cntmsk  = 0xf,
    .code    = 0x2f,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ingress_entry_reject_q1),
    .umasks  = knl_unc_cha_ingress_entry_reject_q1,
  },
  { .name    = "UNC_H_SF_LOOKUP",
    .desc    = "Cache Lookups. Counts the number of times the LLC was accessed.",
    .cntmsk  = 0xf,
    .code    = 0x34,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_llc_lookup),
    .umasks  = knl_unc_cha_llc_lookup,
  },
  { .name    = "UNC_H_CACHE_LINES_VICTIMIZED",
    .desc    = "Cache Lookups. Counts the number of times the LLC was accessed.",
    .cntmsk  = 0xf,
    .code    = 0x37,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_llc_victims),
    .umasks  = knl_unc_cha_llc_victims,
  },
  { .name    = "UNC_H_TOR_INSERTS",
    .desc = "Counts the number of entries successfuly inserted into the TOR that match  qualifications specified by the subevent.",
    .modmsk = KNL_UNC_CHA_TOR_ATTRS,
    .cntmsk  = 0xf,
    .code    = 0x35,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tor_subevent),
    .umasks  = knl_unc_cha_tor_subevent
  },
  { .name    = "UNC_H_TOR_OCCUPANCY",
    .desc    = "For each cycle, this event accumulates the number of valid entries in the TOR that match qualifications specified by the subevent",
    .modmsk = KNL_UNC_CHA_TOR_ATTRS,
    .cntmsk  = 0xf,
    .code    = 0x36,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tor_subevent),
    .umasks  = knl_unc_cha_tor_subevent
  },
  { .name    = "UNC_H_MISC",
    .desc    = "Miscellaneous events in the Cha",
    .cntmsk  = 0xf,
    .code    = 0x39,
    .ngrp    = 1,
    .flags   = INTEL_X86_NO_AUTOENCODE,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_misc),
    .umasks  = knl_unc_cha_misc,
  },
  { .name    = "UNC_H_AG0_AD_CRD_ACQUIRED",
    .desc    = "CMS Agent0 AD Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x80,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_AD_CRD_ACQUIRED_EXT",
    .desc    = "CMS Agent0 AD Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x81,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG0_AD_CRD_OCCUPANCY",
    .desc    = "CMS Agent0 AD Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x82,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_AD_CRD_OCCUPANCY_EXT",
    .desc    = "CMS Agent0 AD Credits Acquired For Transgress.",
    .cntmsk  = 0xf,
    .code    = 0x83,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_AD_CRD_ACQUIRED",
    .desc    = "CMS Agent1 AD Credits Acquired .",
    .cntmsk  = 0xf,
    .code    = 0x84,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_AD_CRD_ACQUIRED_EXT",
    .desc    = "CMS Agent1 AD Credits Acquired .",
    .cntmsk  = 0xf,
    .code    = 0x85,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_AD_CRD_OCCUPANCY",
    .desc    = "CMS Agent1 AD Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x86,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_AD_CRD_OCCUPANCY_EXT",
    .desc    = "CMS Agent1 AD Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x87,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG0_BL_CRD_ACQUIRED",
    .desc    = "CMS Agent0 BL Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x88,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_BL_CRD_ACQUIRED_EXT",
    .desc    = "CMS Agent0 BL Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x89,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG0_BL_CRD_OCCUPANCY",
    .desc    = "CMS Agent0 BL Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x8a,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_BL_CRD_OCCUPANCY_EXT",
    .desc    = "CMS Agent0 BL Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x8b,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_BL_CRD_ACQUIRED",
    .desc    = "CMS Agent1 BL Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x8c,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_BL_CRD_ACQUIRED_EXT",
    .desc    = "CMS Agent1 BL Credits Acquired.",
    .cntmsk  = 0xf,
    .code    = 0x8d,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_BL_CRD_OCCUPANCY",
    .desc    = "CMS Agent1 BL Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x8e,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_BL_CRD_OCCUPANCY_EXT",
    .desc    = "CMS Agent1 BL Credits Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x8f,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG0_STALL_NO_CRD_EGRESS_HORZ_AD",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD0,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_STALL_NO_CRD_EGRESS_HORZ_AD_EXT",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD1,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_STALL_NO_CRD_EGRESS_HORZ_AD",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD2,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_STALL_NO_CRD_EGRESS_HORZ_AD_EXT",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD3,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG0_STALL_NO_CRD_EGRESS_HORZ_BL",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD4,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG0_STALL_NO_CRD_EGRESS_HORZ_BL_EXT",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD5,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_AG1_STALL_NO_CRD_EGRESS_HORZ_BL",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD6,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext),
    .umasks  = knl_unc_cha_tgr_ext,
  },
  { .name    = "UNC_H_AG1_STALL_NO_CRD_EGRESS_HORZ_BL_EXT",
    .desc    = "Stall on No AD Transgress Credits.",
    .cntmsk  = 0xf,
    .code    = 0xD7,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_tgr_ext1),
    .umasks  = knl_unc_cha_tgr_ext1,
  },
  { .name    = "UNC_H_EGRESS_VERT_OCCUPANCY",
    .desc    = "CMS Vert Egress Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x90,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_INSERTS",
    .desc    = "CMS Vert Egress Allocations.",
    .cntmsk  = 0xf,
    .code    = 0x91,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_CYCLES_FULL",
    .desc    = "Cycles CMS Vertical Egress Queue Is Full.",
    .cntmsk  = 0xf,
    .code    = 0x92,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_CYCLES_NE",
    .desc    = "Cycles CMS Vertical Egress Queue Is Not Empty.",
    .cntmsk  = 0xf,
    .code    = 0x93,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_NACK",
    .desc    = "CMS Vertical Egress NACKs.",
    .cntmsk  = 0xf,
    .code    = 0x98,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_STARVED",
    .desc    = "CMS Vertical Egress Injection Starvation.",
    .cntmsk  = 0xf,
    .code    = 0x9a,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_ADS_USED",
    .desc    = "CMS Vertical ADS Used.",
    .cntmsk  = 0xf,
    .code    = 0x9c,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_VERT_BYPASS",
    .desc    = "CMS Vertical Egress Bypass.",
    .cntmsk  = 0xf,
    .code    = 0x9e,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type_agent),
    .umasks  = knl_unc_cha_ring_type_agent,
  },
  { .name    = "UNC_H_EGRESS_HORZ_OCCUPANCY",
    .desc    = "CMS Horizontal Egress Occupancy.",
    .cntmsk  = 0xf,
    .code    = 0x94,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_INSERTS",
    .desc    = "CMS Horizontal Egress Inserts.",
    .cntmsk  = 0xf,
    .code    = 0x95,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_CYCLES_FULL",
    .desc    = "Cycles CMS Horizontal Egress Queue is Full.",
    .cntmsk  = 0xf,
    .code    = 0x96,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_CYCLES_NE",
    .desc    = "Cycles CMS Horizontal Egress Queue is Not Empty.",
    .cntmsk  = 0xf,
    .code    = 0x97,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_NACK",
    .desc    = "CMS Horizontal Egress NACKs.",
    .cntmsk  = 0xf,
    .code    = 0x99,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_STARVED",
    .desc    = "CMS Horizontal Egress Injection Starvation.",
    .cntmsk  = 0xf,
    .code    = 0x9b,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_ADS_USED",
    .desc    = "CMS Horizontal ADS Used.",
    .cntmsk  = 0xf,
    .code    = 0x9d,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_EGRESS_HORZ_BYPASS",
    .desc    = "CMS Horizontal Egress Bypass.",
    .cntmsk  = 0xf,
    .code    = 0x9f,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_RING_BOUNCES_VERT",
    .desc    = "Number of incoming messages from the Vertical ring that were bounced, by ring type.",
    .cntmsk  = 0xf,
    .code    = 0xa0,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_RING_BOUNCES_HORZ",
    .desc    = "Number of incoming messages from the Horizontal ring that were bounced, by ring type.",
    .cntmsk  = 0xf,
    .code    = 0xa1,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_RING_SINK_STARVED_VERT",
    .desc    = "Vertical ring sink starvation count.",
    .cntmsk  = 0xf,
    .code    = 0xa2,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_RING_SINK_STARVED_HORZ",
    .desc    = "Horizontal ring sink starvation count.",
    .cntmsk  = 0xf,
    .code    = 0xa3,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_type),
    .umasks  = knl_unc_cha_ring_type,
  },
  { .name    = "UNC_H_RING_SRC_THRT",
    .desc    = "Counts cycles in throttle mode.",
    .cntmsk  = 0xf,
    .code    = 0xa4,
  },
  { .name    = "UNC_H_FAST_ASSERTED",
    .desc    = "Counts cycles source throttling is adderted",
    .cntmsk  = 0xf,
    .code    = 0xa5,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_dire_ext),
    .umasks  = knl_unc_cha_dire_ext,
  },
  { .name    = "UNC_H_VERT_RING_AD_IN_USE",
    .desc    = "Counts the number of cycles that the Vertical AD ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xa6,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_vert),
    .umasks  = knl_unc_cha_ring_use_vert,
  },
  { .name    = "UNC_H_HORZ_RING_AD_IN_USE",
    .desc    = "Counts the number of cycles that the Horizontal AD ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xa7,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_hori),
    .umasks  = knl_unc_cha_ring_use_hori,
  },
  { .name    = "UNC_H_VERT_RING_AK_IN_USE",
    .desc    = "Counts the number of cycles that the Vertical AK ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xa8,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_vert),
    .umasks  = knl_unc_cha_ring_use_vert,
  },
  { .name    = "UNC_H_HORZ_RING_AK_IN_USE",
    .desc    = "Counts the number of cycles that the Horizontal AK ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xa9,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_hori),
    .umasks  = knl_unc_cha_ring_use_hori,
  },
  { .name    = "UNC_H_VERT_RING_BL_IN_USE",
    .desc    = "Counts the number of cycles that the Vertical BL ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xaa,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_vert),
    .umasks  = knl_unc_cha_ring_use_vert,
  },
  { .name    = "UNC_H_HORZ_RING_BL_IN_USE",
    .desc    = "Counts the number of cycles that the Horizontal BL ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xab,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_hori),
    .umasks  = knl_unc_cha_ring_use_hori,
  },
  { .name    = "UNC_H_VERT_RING_IV_IN_USE",
    .desc    = "Counts the number of cycles that the Vertical IV ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xac,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_updn),
    .umasks  = knl_unc_cha_ring_use_updn,
  },
  { .name    = "UNC_H_HORZ_RING_IV_IN_USE",
    .desc    = "Counts the number of cycles that the Horizontal IV ring is being used at this ring stop.",
    .cntmsk  = 0xf,
    .code    = 0xad,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_ring_use_lfrt),
    .umasks  = knl_unc_cha_ring_use_lfrt,
  },
  { .name    = "UNC_H_EGRESS_ORDERING",
    .desc    = "Counts number of cycles IV was blocked in the TGR Egress due to SNP/GO Ordering requirements.",
    .cntmsk  = 0xf,
    .code    = 0xae,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_iv_snp),
    .umasks  = knl_unc_cha_iv_snp,
  },
  { .name    = "UNC_H_TG_INGRESS_OCCUPANCY",
    .desc    = "Transgress Ingress Occupancy. Occupancy event for the Ingress buffers in the CMS  The Ingress is used to queue up requests received from the mesh.",
    .cntmsk  = 0xf,
    .code    = 0xb0,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_cms_ext),
    .umasks  = knl_unc_cha_cms_ext,
  },
  { .name    = "UNC_H_TG_INGRESS_INSERTS",
    .desc    = "Transgress Ingress Allocations. Number of allocations into the CMS Ingress  The Ingress is used to queue up requests received from the mesh.",
    .cntmsk  = 0xf,
    .code    = 0xb1,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_cms_ext),
    .umasks  = knl_unc_cha_cms_ext,
  },
  { .name    = "UNC_H_TG_INGRESS_BYPASS",
    .desc    = "Transgress Ingress Bypass. Number of packets bypassing the CMS Ingress.",
    .cntmsk  = 0xf,
    .code    = 0xb2,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_cms_ext),
    .umasks  = knl_unc_cha_cms_ext,
  },
  { .name    = "UNC_H_TG_INGRESS_CRD_STARVED",
    .desc    = "Transgress Injection Starvation. Counts cycles under injection starvation mode.  This starvation is triggered when the CMS Ingress cannot send a transaction onto the mesh for a long period of time.  In this case, the Ingress is unable to forward to the Egress due to a lack of credit.",
    .cntmsk  = 0xf,
    .code    = 0xb3,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_cms_crd_starved),
    .umasks  = knl_unc_cha_cms_crd_starved,
  },
  { .name    = "UNC_H_TG_INGRESS_BUSY_STARVED",
    .desc    = "Transgress Injection Starvation. Counts cycles under injection starvation mode.  This starvation is triggered when the CMS Ingress cannot send a transaction onto the mesh for a long period of time.  In this case, because a message from the other queue has higher priority.",
    .cntmsk  = 0xf,
    .code    = 0xb4,
    .ngrp    = 1,
    .numasks = LIBPFM_ARRAY_SIZE(knl_unc_cha_cms_busy_starved),
    .umasks  = knl_unc_cha_cms_busy_starved,
  },
};
