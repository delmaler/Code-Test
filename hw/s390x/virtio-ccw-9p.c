/*
 * virtio ccw 9p implementation
 *
 * Copyright 2012, 2015 IBM Corp.
 * Author(s): Pierre Morel <pmorel@linux.vnet.ibm.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or (at
 * your option) any later version. See the COPYING file in the top-level
 * directory.
 */

#include "qemu/osdep.h"
#include "hw/qdev-properties.h"
#include "hw/virtio/virtio.h"
#include "qapi/error.h"
#include "qemu/module.h"
#include "virtio-ccw.h"

static void virtio_ccw_9p_realize(VirtioCcwDevice *ccw_dev, Error **errp)
{
    V9fsCCWState *dev = VIRTIO_9P_CCW(ccw_dev);
    DeviceState *vdev = DEVICE(&dev->vdev);

    qdev_set_parent_bus(vdev, BUS(&ccw_dev->bus));
    object_property_set_bool(OBJECT(vdev), true, "realized", errp);
}

static void virtio_ccw_9p_instance_init(Object *obj)
{
    V9fsCCWState *dev = VIRTIO_9P_CCW(obj);

    virtio_instance_init_common(obj, &dev->vdev, sizeof(dev->vdev),
                                TYPE_VIRTIO_9P);
}

static Property virtio_ccw_9p_properties[] = {
    DEFINE_PROP_BIT("ioeventfd", VirtioCcwDevice, flags,
            VIRTIO_CCW_FLAG_USE_IOEVENTFD_BIT, true),
    DEFINE_PROP_UINT32("max_revision", VirtioCcwDevice, max_rev,
                       VIRTIO_CCW_MAX_REV),
    DEFINE_PROP_END_OF_LIST(),
};

static void virtio_ccw_9p_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtIOCCWDeviceClass *k = VIRTIO_CCW_DEVICE_CLASS(klass);

    k->realize = virtio_ccw_9p_realize;
    dc->props = virtio_ccw_9p_properties;
    set_bit(DEVICE_CATEGORY_STORAGE, dc->categories);
}

static const TypeInfo virtio_ccw_9p_info = {
    .name          = TYPE_VIRTIO_9P_CCW,
    .parent        = TYPE_VIRTIO_CCW_DEVICE,
    .instance_size = sizeof(V9fsCCWState),
    .instance_init = virtio_ccw_9p_instance_init,
    .class_init    = virtio_ccw_9p_class_init,
};

static void virtio_ccw_9p_register(void)
{
    type_register_static(&virtio_ccw_9p_info);
}

type_init(virtio_ccw_9p_register)
