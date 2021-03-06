#include <vdev.h>
#define DEBUG
#include <log/print.h>

//gicc
#define IC_RPI2_BASE_ADDR 0x3F00B000

#define IC_OFFSET_BASEIC_PENDING	0x200
#define IC_OFFSET_PENDING1			0x204
#define IC_OFFSET_PENDING2			0x208
#define IC_OFFSET_FIQ_CONTROL		0x20c
#define IC_OFFSET_ENABLE_IRQS1		0x210
#define IC_OFFSET_ENABLE_IRQS2		0x214
#define IC_OFFSET_ENABLE_BASIC_IRQS	0x218
#define IC_OFFSET_DISABLE_IRQS1		0x21c
#define IC_OFFSET_DISABLE_IRQS2		0x220
#define IC_OFFSET_DISABLE_BASIC_IRQS	0x224



struct ic_rpi2_regs {
	uint32_t intNumber;
//	uint32_t IC_LOCAL_PENDING;
    uint32_t IC_BASEIC_PENDING; /* 0x200  */
    uint32_t IC_PENDING1; /* 0x204 */

    uint32_t IC_PENDING2; /* 0x208 */
    uint32_t IC_FIQ_CONTROL; /* 0x20C */
    uint32_t IC_ENABLE_IRQS1; /* 0x210 */

    uint32_t IC_ENABLE_IRQS2; /* 0x214 */
    uint32_t IC_ENABLE_BASIC_IRQS; /* 0x218 */
    uint32_t IC_DISABLE_IRQS1; /* 0x21C */
    uint32_t IC_DISABLE_IRQS2; /* 0x220 */
    uint32_t IC_DISABLE_BASIC_IRQS; /* 0x224  */

};

struct ic_rpi2_handler_entry {
    uint32_t offset;
    vdev_callback_t handler;
};
#define PENDING_MAX 20
static struct ic_rpi2_regs ci_regs[NUM_GUESTS_STATIC];
static struct ic_rpi2_regs ci_pending_regs[PENDING_MAX];
static int countPending = 0;

static struct vdev_memory_map _vdev_ic_rpi2_info = {
   .base = IC_RPI2_BASE_ADDR,
   .size = 0x00001000,
};

static hvmm_status_t handler_null(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	printH("cpu interface handler_null\n");
	return HVMM_STATUS_BAD_ACCESS;
}
static hvmm_status_t handler_F(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	return HVMM_STATUS_BAD_ACCESS;
}
static hvmm_status_t handler_E(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	return HVMM_STATUS_BAD_ACCESS;
}
static hvmm_status_t handler_D(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	return HVMM_STATUS_BAD_ACCESS;
}
static hvmm_status_t handler_2(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	return HVMM_STATUS_BAD_ACCESS;
}
static hvmm_status_t handler_1(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	return HVMM_STATUS_BAD_ACCESS;
}


static hvmm_status_t handler_0(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{
	/* CTLR, PMR, BPR, IAR */
    hvmm_status_t result = HVMM_STATUS_BAD_ACCESS;
//    vmid_t vmid = guest_current_vmid();
//    struct ic_rpi2_regs *regs = &ci_regs[vmid];
//    uint32_t woffset = offset / 4;
//    switch (woffset) {
//    case IC_OFFSET_BASEIC_PENDING: /* RW */
//        if (write)
//            regs->GICC_CTLR = *pvalue;
//        else
//            *pvalue = regs->GICC_CTLR;
//        result = HVMM_STATUS_SUCCESS;
//        break;
//    case GIC_OFFSET_GICC_PMR: /* RO */
//        if (write) {
//            regs->GICC_PMR = *pvalue;
//        } else
//        	*pvalue = regs->GICC_PMR;
//
//        result = HVMM_STATUS_SUCCESS;
//        break;
//    case GIC_OFFSET_GICC_BPR: /* RO */
//        if (write) {
//            regs->GICC_BPR = *pvalue;
//        } else
//        	*pvalue = regs->GICC_BPR;
//        result = HVMM_STATUS_SUCCESS;
//        break;
//    case GIC_OFFSET_GICC_IAR:
//        if (write) {
//            regs->GICC_IAR = *pvalue;
//        } else
//        	*pvalue = regs->GICC_IAR;
//        result = HVMM_STATUS_SUCCESS;
//    	break;
////    default: { /* RW GICD_IGROUPR */
////        int igroup = woffset - GICD_IGROUPR;
////        if (igroup >= 0 && igroup < VGICD_NUM_IGROUPR) {
////            if (write)
////                regs->IGROUPR[igroup] = *pvalue;
////            else
////                *pvalue = regs->IGROUPR[igroup];
////
////            result = HVMM_STATUS_SUCCESS;
////        }
////    }
////        break;
//    }
//    if (result != HVMM_STATUS_SUCCESS)
//        printH("vgicd: invalid access offset:%x write:%d\n", offset,
//                write);

    return result;
}
static struct ic_rpi2_handler_entry _ci_handler_map[0x10] = {
/* 0x00 ~ 0x0F */
{ 0x00, handler_0 }, /* CTLR, PMR, BPR, IAR */
{ 0x01, handler_1 }, /* EOIR, RPR, HPPIR, ABPR */
{ 0x02, handler_2 }, /* AIAR, AEOIR, AHPPIR */
{ 0x03, handler_null},
{ 0x04, handler_null},
{ 0x05, handler_null},
{ 0x06, handler_null},
{ 0x07, handler_null},
{ 0x08, handler_null},
{ 0x09, handler_null},
{ 0x0A, handler_null},
{ 0x0B, handler_null},
{ 0x0C, handler_null},
{ 0x0D, handler_D}, /* APRN */
{ 0x0E, handler_E }, /* NSAPRN */
{ 0x0F, handler_F }, /* IIDR */
//{ 0x0F, handler_F }, /* IIDR */
};
//static hvmm_status_t vdev_ic_rpi2_access_handler(uint32_t write, uint32_t offset,
//        uint32_t *pvalue, enum vdev_access_size access_size)
//{
//    printH("%s: %s offset:%x value:%x access_size : %d\n", __func__,
//            write ? "write" : "read", offset,
//            write ? *pvalue : (uint32_t) pvalue, access_size);
//    hvmm_status_t result = HVMM_STATUS_BAD_ACCESS;
//
//
//    // DIR
//
//    uint8_t offsetidx = (uint8_t) ((offset & 0xF0) >> 4);
//    printH("offset_idx : %d\n", offsetidx);
//    result = _ci_handler_map[offsetidx].handler(write, offset, pvalue,
//            access_size);
//
//    return result;
//}


#define IC_OFFSET_BASEIC_PENDING	0x200
#define IC_OFFSET_PENDING1			0x204
#define IC_OFFSET_PENDING2			0x208
#define IC_OFFSET_FIQ_CONTROL		0x20c
#define IC_OFFSET_ENABLE_IRQS1		0x210
#define IC_OFFSET_ENABLE_IRQS2		0x214
#define IC_OFFSET_ENABLE_BASIC_IRQS	0x218
#define IC_OFFSET_DISABLE_IRQS1		0x21c
#define IC_OFFSET_DISABLE_IRQS2		0x220
#define IC_OFFSET_DISABLE_BASIC_IRQS	0x224

int isFirst = 0;

static hvmm_status_t vdev_ic_rpi2_access_handler(uint32_t write, uint32_t offset,
        uint32_t *pvalue, enum vdev_access_size access_size)
{

    hvmm_status_t result = HVMM_STATUS_BAD_ACCESS;
    unsigned int vmid = guest_current_vmid();
    volatile int *addr = (IC_RPI2_BASE_ADDR + offset);
//    volatile int *daddr = (IC_RPI2_BASE_ADDR + GIC_OFFSET_GICC_DIR);
//    if (offset == 20)
//    	printH("iar = %d\n", *addr & 0x03ff);
//    if (offset == 24)

    if (!write) {
        /* READ */
    	if(offset == IC_OFFSET_BASEIC_PENDING)
    	{
    		*pvalue = ci_regs[0].IC_BASEIC_PENDING;
    	} else if (offset == IC_OFFSET_PENDING1) {
    		*pvalue = ci_regs[0].IC_PENDING1;

		} else if (offset == IC_OFFSET_PENDING2) {
			*pvalue = ci_regs[0].IC_PENDING2;
		} else if (offset == IC_OFFSET_FIQ_CONTROL) {
			*pvalue = ci_regs[0].IC_FIQ_CONTROL;
		} else if (offset == IC_OFFSET_ENABLE_IRQS1) {
			*pvalue = ci_regs[0].IC_ENABLE_IRQS1;
		} else if (offset == IC_OFFSET_ENABLE_IRQS2) {
			*pvalue = ci_regs[0].IC_ENABLE_IRQS2;
		} else if (offset == IC_OFFSET_ENABLE_BASIC_IRQS) {
			*pvalue = ci_regs[0].IC_ENABLE_BASIC_IRQS;
		} else if (offset == IC_OFFSET_DISABLE_IRQS1) {
			*pvalue = ci_regs[0].IC_DISABLE_IRQS1;
		} else if (offset == IC_OFFSET_DISABLE_IRQS2) {
			*pvalue = ci_regs[0].IC_DISABLE_IRQS2;
		} else if (offset == IC_OFFSET_DISABLE_BASIC_IRQS) {
			*pvalue = ci_regs[0].IC_DISABLE_BASIC_IRQS;
		} else
			*pvalue =  (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR + offset)));

//    	printH("1111111111111 %x\n", *pvalue);
       } else {
    	    /* WRITE */
//    	   if ( (offset == IC_OFFSET_ENABLE_IRQS2) && (0x02000000 == *pvalue) && (isFirst==1))
//    		   ;
//    	   else if ( (offset == IC_OFFSET_ENABLE_IRQS2) && (0x02000000 == *pvalue) && (isFirst==0)) {
//    		   *addr = *pvalue;
//    		   isFirst = 1 ;
//    	   }
//    	   else
    		   *addr = *pvalue;
//    		   *addr = *pvalue;
//    		   *addr = *pvalue;

    		    if(0x02000000 != *pvalue)
    		    	printH("%s: %s offset:%x value:%x\n", __func__,
    		            write ? "write" : "read", offset,
    		            write ? *pvalue : *pvalue);
       }
//    printH("access size : %d\n", access_size);

//    printH("AAAAAAAAAAAAAA %x\n", *addr);
    	result = HVMM_STATUS_SUCCESS;
    return result;
}
static hvmm_status_t vdev_ic_rpi2_read(struct arch_vdev_trigger_info *info,
                        struct arch_regs *regs)
{
    uint32_t offset = info->fipa - _vdev_ic_rpi2_info.base;

    return vdev_ic_rpi2_access_handler(0, offset, info->value, info->sas);
}

static hvmm_status_t vdev_ic_rpi2_write(struct arch_vdev_trigger_info *info,
                        struct arch_regs *regs)
{
    uint32_t offset = info->fipa - _vdev_ic_rpi2_info.base;

    return vdev_ic_rpi2_access_handler(1, offset, info->value, info->sas);
}

static int32_t vdev_ic_rpi2_post(struct arch_vdev_trigger_info *info,
                        struct arch_regs *regs)
{
    uint8_t isize = 4;

    if (regs->cpsr & 0x20) /* Thumb */
        isize = 2;

    regs->pc += isize;

    return 0;
}

static int32_t vdev_ic_rpi2_check(struct arch_vdev_trigger_info *info,
                        struct arch_regs *regs)
{
    uint32_t offset = info->fipa - _vdev_ic_rpi2_info.base;

    if (info->fipa >= _vdev_ic_rpi2_info.base &&
        offset < _vdev_ic_rpi2_info.size)
        return 0;
    /**
     * Find vdev using tag.  Hard coding.
     */
    if( !regs ) {
    	return 66;
    }
    return VDEV_NOT_FOUND;
}






static hvmm_status_t vdev_ic_rpi2_reset(void)
{


    printH("vdev init:'%s'\n", __func__);
    int j = 0;
    int i = 0;

    for (i = 0; i < 1; i++) {

    	ci_regs[i].IC_BASEIC_PENDING =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_BASEIC_PENDING)));
    	ci_regs[i].IC_PENDING1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING1)));

    	ci_regs[i].IC_PENDING2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING2)));

    	ci_regs[i].IC_FIQ_CONTROL =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_FIQ_CONTROL)));
    	ci_regs[i].IC_ENABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS1)));
    	ci_regs[i].IC_ENABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS2)));
    	ci_regs[i].IC_ENABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_BASIC_IRQS)));
    	ci_regs[i].IC_DISABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS1)));

    	ci_regs[i].IC_DISABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS2)));
    	ci_regs[i].IC_DISABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_BASIC_IRQS)));

//    	static struct ic_rpi2_regs ci_pending_regs[PENDING_MAX];
//    	static int countPending = 0;
    }
    for (i = 0; i < PENDING_MAX ; i++) {

    	ci_pending_regs[i].IC_BASEIC_PENDING =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_BASEIC_PENDING)));
    	ci_pending_regs[i].IC_PENDING1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING1)));

    	ci_pending_regs[i].IC_PENDING2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING2)));

    	ci_pending_regs[i].IC_FIQ_CONTROL =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_FIQ_CONTROL)));
    	ci_pending_regs[i].IC_ENABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS1)));
    	ci_pending_regs[i].IC_ENABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS2)));
    	ci_pending_regs[i].IC_ENABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_BASIC_IRQS)));
    	ci_pending_regs[i].IC_DISABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS1)));

    	ci_pending_regs[i].IC_DISABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS2)));
    	ci_pending_regs[i].IC_DISABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_BASIC_IRQS)));

//    	static struct ic_rpi2_regs ci_pending_regs[PENDING_MAX];
//    	static int countPending = 0;
    }

    return HVMM_STATUS_SUCCESS;
}

static hvmm_status_t vdev_ic_rpi2_execute(int level, int num, int type, int irq)
{
	volatile int *addr;
	hvmm_status_t result = HVMM_STATUS_BAD_ACCESS;

//	 printH("vdev_ic_rpi2_execute: irq: %d, type : %d\n", irq, type);
	if (type == 0) { // EOI
		ci_regs[0].IC_BASEIC_PENDING = irq;
	    return HVMM_STATUS_SUCCESS;

	} else if (type == 1) {  // inject

		ci_regs[0].IC_PENDING1 = irq;
//		printH("vdev_ic_rpi2_execute, IC_PENDING1 inject-ok: irq: %d\n", irq);


	} else if (type == 2) {
		ci_regs[0].IC_PENDING2 = irq;
//		printH("vdev_ic_rpi2_execute, IC_PENDING2 inject-ok: irq: %d\n", irq);


	} else if (type == 3) { //all coply
//		printH("All Copy IC_pri2\n");

    	ci_regs[0].IC_BASEIC_PENDING =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_BASEIC_PENDING)));
    	ci_regs[0].IC_PENDING1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING1)));

    	ci_regs[0].IC_PENDING2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING2)));

    	ci_regs[0].IC_FIQ_CONTROL =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_FIQ_CONTROL)));
    	ci_regs[0].IC_ENABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS1)));
    	ci_regs[0].IC_ENABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS2)));
    	ci_regs[0].IC_ENABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_BASIC_IRQS)));
    	ci_regs[0].IC_DISABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS1)));

    	ci_regs[0].IC_DISABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS2)));
    	ci_regs[0].IC_DISABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_BASIC_IRQS)));

		return HVMM_STATUS_SUCCESS;
	}
	else if (type == 4) { //all coply
	//		printH("All Copy IC_pri2\n");
//			* (int*) 40000060 = 0x110;

	    	ci_regs[0].IC_BASEIC_PENDING = 0x80000 ;// 0x02000000;//0x80000; //uart

	    	ci_regs[0].IC_PENDING1 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_PENDING1)));

	    	ci_regs[0].IC_PENDING2 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_PENDING2)));

	    	ci_regs[0].IC_FIQ_CONTROL =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_FIQ_CONTROL)));
	    	ci_regs[0].IC_ENABLE_IRQS1 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_ENABLE_IRQS1)));
	    	ci_regs[0].IC_ENABLE_IRQS2 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_ENABLE_IRQS2)));
	    	ci_regs[0].IC_ENABLE_BASIC_IRQS =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_ENABLE_BASIC_IRQS)));
	    	ci_regs[0].IC_DISABLE_IRQS1 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_DISABLE_IRQS1)));

	    	ci_regs[0].IC_DISABLE_IRQS2 =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_DISABLE_IRQS2)));
	    	ci_regs[0].IC_DISABLE_BASIC_IRQS =
	                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
	                        + IC_OFFSET_DISABLE_BASIC_IRQS)));


//
//	    	if(irq == 65)
//	    		*((int*)0x3F00B224) = 0x2;
//	    	if(irq == 66 )
//	    		 *((int*)0x3F00B224) = 0x4;
//	    	if(irq == 9999)
//	    		*((int*)0x3F00B220) = 0x02000000;
		}
	//add pending irq
	else if (type == 5) {
		if (countPending > PENDING_MAX)
		{
			printH("Pending MAX\n");
			return HVMM_STATUS_SUCCESS;
		}

//		printH("inject : %d\n", ci_pending_regs[countPending].intNumber);
		ci_pending_regs[countPending].intNumber = irq;

		if(irq == 65)
			ci_pending_regs[countPending].IC_BASEIC_PENDING = 0x2;
		else if(irq == 66)
			ci_pending_regs[countPending].IC_BASEIC_PENDING = 0x4;
		else if(irq == 9999)
			ci_pending_regs[countPending].IC_BASEIC_PENDING = 0x80000;
		else {
			ci_pending_regs[countPending].IC_BASEIC_PENDING =
					(uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
					                        + IC_OFFSET_BASEIC_PENDING)));
		}

		ci_pending_regs[countPending].IC_PENDING1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING1)));

		ci_pending_regs[countPending].IC_PENDING2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_PENDING2)));

		ci_pending_regs[countPending].IC_FIQ_CONTROL =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_FIQ_CONTROL)));
		ci_pending_regs[countPending].IC_ENABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS1)));
		ci_pending_regs[countPending].IC_ENABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_IRQS2)));
		ci_pending_regs[countPending].IC_ENABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_ENABLE_BASIC_IRQS)));
		ci_pending_regs[countPending].IC_DISABLE_IRQS1 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS1)));

		ci_pending_regs[countPending].IC_DISABLE_IRQS2 =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_IRQS2)));
		ci_pending_regs[countPending].IC_DISABLE_BASIC_IRQS =
                (uint32_t) (*((volatile unsigned int*) (IC_RPI2_BASE_ADDR
                        + IC_OFFSET_DISABLE_BASIC_IRQS)));
		countPending++;

		return HVMM_STATUS_SUCCESS;
	} else if (type == 6) {
		if(countPending < 1) {
			printH("pending is null\n");
			return HVMM_STATUS_BAD_ACCESS;
		}
		countPending--;
//		printH("Copy Pending IC register :%d\n", ci_pending_regs[countPending].intNumber);
    	ci_regs[0].IC_BASEIC_PENDING = ci_pending_regs[countPending].IC_BASEIC_PENDING;
    	ci_regs[0].IC_PENDING1 = ci_pending_regs[countPending].IC_PENDING1;
    	ci_regs[0].IC_PENDING2 = ci_pending_regs[countPending].IC_PENDING2;
    	ci_regs[0].IC_FIQ_CONTROL = ci_pending_regs[countPending].IC_FIQ_CONTROL;
    	ci_regs[0].IC_ENABLE_IRQS1 = ci_pending_regs[countPending].IC_ENABLE_IRQS1;
    	ci_regs[0].IC_ENABLE_IRQS2 =ci_pending_regs[countPending].IC_ENABLE_IRQS2;
    	ci_regs[0].IC_ENABLE_BASIC_IRQS = ci_pending_regs[countPending].IC_ENABLE_BASIC_IRQS;
    	ci_regs[0].IC_DISABLE_IRQS1 = ci_pending_regs[countPending].IC_DISABLE_IRQS1;
    	ci_regs[0].IC_DISABLE_IRQS2 = ci_pending_regs[countPending].IC_DISABLE_IRQS2;
    	ci_regs[0].IC_DISABLE_BASIC_IRQS = ci_pending_regs[countPending].IC_DISABLE_BASIC_IRQS;


//    	printH("IC_BASEIC_PENDING:%x\n",ci_regs[0].IC_BASEIC_PENDING );
//    	printH("IC_PENDING1:%x\n",ci_regs[0].IC_PENDING1);
//    	printH("IC_PENDING2:%x\n",ci_regs[0].IC_PENDING2);
//    	printH("IC_FIQ_CONTROL:%x\n",ci_regs[0].IC_FIQ_CONTROL);
//    	printH("IC_ENABLE_IRQS1:%x\n",ci_regs[0].IC_ENABLE_IRQS1);
//    	printH("IC_ENABLE_IRQS2:%x\n",ci_regs[0].IC_ENABLE_IRQS2);
//    	printH("IC_ENABLE_BASIC_IRQS:%x\n",ci_regs[0].IC_ENABLE_BASIC_IRQS);
//    	printH("IC_DISABLE_IRQS1:%x\n",ci_regs[0].IC_DISABLE_IRQS1);
//    	printH("IC_DISABLE_IRQS2:%x\n",ci_regs[0].IC_DISABLE_IRQS2);
//    	printH("IC_DISABLE_BASIC_IRQS:%x\n",ci_regs[0].IC_DISABLE_BASIC_IRQS);



    	return HVMM_STATUS_SUCCESS;
	} else if (type == 7) {
		return countPending;
	}
	return HVMM_STATUS_SUCCESS;
}

struct vdev_ops _vdev_ic_rpi2_ops = {
    .init = vdev_ic_rpi2_reset,
    .check = vdev_ic_rpi2_check,
    .read = vdev_ic_rpi2_read,
    .write = vdev_ic_rpi2_write,
    .post = vdev_ic_rpi2_post,
    .execute = vdev_ic_rpi2_execute,
};

struct vdev_module _vdev_ic_rpi2_module = {
    .name = "K-Hypervisor vDevice Interrupt Controller RPI2 Module",
    .author = "Kookmin Univ.",
    .ops = &_vdev_ic_rpi2_ops,
    .tag = 66,
};

hvmm_status_t vdev_ic_rpi2()
{
    hvmm_status_t result = HVMM_STATUS_BUSY;

    result = vdev_register(VDEV_LEVEL_LOW, &_vdev_ic_rpi2_module);
    if (result == HVMM_STATUS_SUCCESS)
        printH("vdev registered:'%s'\n", _vdev_ic_rpi2_module.name);
    else {
        printh("%s: Unable to register vdev:'%s' code=%x\n",
                __func__, _vdev_ic_rpi2_module.name, result);
    }

    return result;
}
vdev_module_low_init(vdev_ic_rpi2);
