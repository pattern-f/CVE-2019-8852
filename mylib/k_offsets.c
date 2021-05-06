//
//  k_offsets.c
//  ios-fuzzer
//
//  Created by Quote on 2021/1/26.
//  Copyright © 2021 Quote. All rights reserved.
//

#include <string.h>
#include "mycommon.h"
#include "utils.h"
#define Q_INTERNAL
#include "k_offsets.h"

static void offsets_base_macOS_10_15_x()
{
    kc_kernel_base = 0xFFFFFFF007004000;

    SIZE(ipc_entry)              = 0x18;
    OFFSET(ipc_entry, ie_object) =  0x0;
    OFFSET(ipc_entry, ie_bits)   =  0x8;

    OFFSET(ipc_port, ip_bits)       =  0x0;
    OFFSET(ipc_port, ip_references) =  0x4;
    OFFSET(ipc_port, ip_receiver)   = 0x60;
    OFFSET(ipc_port, ip_kobject)    = 0x68;
    OFFSET(ipc_port, ip_mscount)    = 0x9c;
    OFFSET(ipc_port, ip_srights)    = 0xa0;

    OFFSET(ipc_space, is_table_size) = 0x0c; // different with iOS
    OFFSET(ipc_space, is_table)      = 0x18; // diff
    OFFSET(ipc_space, is_task)       = 0x20; // diff

    OFFSET(task, lck_mtx_data)        =   0x0; // lck_mtx_t (i386/arm is not the same)
    OFFSET(task, ref_count)           =  0x10;
    OFFSET(task, active)              =  0x14;
    OFFSET(task, map)                 =  0x28;
    OFFSET(task, itk_sself)           = 0x108;
    OFFSET(task, itk_space)           = 0x320;
    OFFSET(task, bsd_info)            = 0x3a0; // diff

    OFFSET(proc, le_next) = 0x00;
    OFFSET(proc, le_prev) = 0x08;
    OFFSET(proc, task) = 0x10;
    OFFSET(proc, p_pid) = 0x68;
    OFFSET(proc, p_ucred) = 0xf0;
    OFFSET(proc, p_fd) = 0xf8;

    OFFSET(filedesc, fd_ofiles) = 0x00;
    OFFSET(fileproc, fp_glob) = 0x10;
    OFFSET(fileglob, fg_data) = 0x38;
    OFFSET(pipe, buffer) = 0x10;

    OFFSET(ucred, cr_posix) = 0x18;

    SIZE(posix_cred) = 0x60;

    OFFSET(OSDictionary, count)      = 0x14;
    OFFSET(OSDictionary, capacity)   = 0x18;
    OFFSET(OSDictionary, dictionary) = 0x20;

    OFFSET(OSString, string) = 0x10;

    OFFSET(IOSurfaceRootUserClient, surfaceClients) = 0x118;
}

static void offsets_base_iOS_14_x()
{
    kc_kernel_base = 0xFFFFFFF007004000;

    SIZE(ipc_entry)              = 0x18;
    OFFSET(ipc_entry, ie_object) =  0x0;

    OFFSET(ipc_port, ip_bits)       =  0x0;
    OFFSET(ipc_port, ip_references) =  0x4;
    OFFSET(ipc_port, ip_kobject)    = 0x68;

    OFFSET(ipc_space, is_table_size) = 0x14;
    OFFSET(ipc_space, is_table)      = 0x20;

    OFFSET(task, map) = 0x28;
    OFFSET(task, itk_space) = 0x330;
#if __arm64e__
    OFFSET(task, bsd_info) = 0x3a0;
    OFFSET(task, t_flags) = 0x3f4;
#else
    OFFSET(task, bsd_info) = 0x390;
    OFFSET(task, t_flags) = 0x3d8;
#endif

    OFFSET(proc, le_next) = 0x00;
    OFFSET(proc, le_prev) = 0x08;
    OFFSET(proc, task) = 0x10;
    OFFSET(proc, p_pid) = 0x68;
    OFFSET(proc, p_ucred) = 0xf0;
    OFFSET(proc, p_fd) = 0xf8;

    OFFSET(filedesc, fd_ofiles) = 0x00;
    OFFSET(fileproc, fp_glob) = 0x10;
    OFFSET(fileglob, fg_data) = 0x38;
    OFFSET(pipe, buffer) = 0x10;

    OFFSET(ucred, cr_posix) = 0x18;

    SIZE(posix_cred) = 0x60;

    OFFSET(OSDictionary, count)      = 0x14;
    OFFSET(OSDictionary, capacity)   = 0x18;
    OFFSET(OSDictionary, dictionary) = 0x20;

    OFFSET(OSString, string) = 0x10;

    OFFSET(IOSurfaceRootUserClient, surfaceClients) = 0x118;
}

static void offsets_iPhone6s_18A373()
{
    offsets_base_iOS_14_x();
}

static void offsets_iPhone11_18A373()
{
    offsets_base_iOS_14_x();

    kc_kernel_map = 0xFFFFFFF0076DA618;
    kc_kernel_task = 0xFFFFFFF0076D6998;
    kc_IOSurfaceClient_vt = 0xFFFFFFF00783CDA8;
    kc_IOSurfaceClient_vt_0 = 0xFFFFFFF00867A778;
}

static void offsets_iPhone12pro_18C66()
{
    offsets_base_iOS_14_x();
}

struct device_def {
    const char *name;
    const char *model;
    const char *build;
    void (*init)(void);
};

static struct device_def devices[] = {
    { "iPhone 6s", "N71AP", "18A373", offsets_iPhone6s_18A373 },
    { "iPhone 11", "N104AP", "18A373", offsets_iPhone11_18A373 },
    { "iPhone 12 pro", "D53pAP", "18C66", offsets_iPhone12pro_18C66 },
};

void kernel_offsets_init(void)
{
    for (int i = 0; i < arrayn(devices); i++) {
        struct device_def *dev = &devices[i];
        if (!strcmp(g_exp.model, dev->model) && !strcmp(g_exp.osversion, dev->build)) {
            dev->init();
            return;
        }
    }
//    util_warning("no device defination, use offsets_base_iOS_14_x");
//    offsets_base_iOS_14_x();
    util_warning("no device defination, use offsets_base_macOS_10_15_x");
    offsets_base_macOS_10_15_x();
}
