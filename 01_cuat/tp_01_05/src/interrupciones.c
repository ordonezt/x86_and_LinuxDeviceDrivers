//https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Extended-Asm

__attribute__(( section(".interrupciones"))) void Divide_Error_Handler(void)
{
    asm("mov $0x0, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Debug_Exception_Handler(void)
{
    asm("mov $0x1, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void NMI_Handler(void)
{
    asm("mov $0x2, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Breakpoint_Handler(void)
{
    asm("mov $0x3, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Overflow_Handler(void)
{
    asm("mov $0x4, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void BR_Handler(void)
{
    asm("mov $0x5, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Invalis_Opcode_Handler(void)
{
    asm("mov $0x6, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void NM_Handler(void)
{
    asm("mov $0x7, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Double_Fault_Handler(void)
{
    asm("mov $0x8, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Invalid_TSS_Handler(void)
{
    asm("mov $0xA, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void NP_Handler(void)
{
    asm("mov $0xB, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void SS_Handler(void)
{
    asm("mov $0xC, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void General_Protection_Handler(void)
{
    asm("mov $0xD, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Page_Fault_Handler(void)
{
    asm("mov $0xE, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void MF_Handler(void)
{
    asm("mov $0x10, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Alignment_Check_Handler(void)
{
    asm("mov $0x11, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Machine_Check_Handler(void)
{
    asm("mov $0x12, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void XM_Handler(void)
{
    asm("mov $0x13, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Virtualization_Exception_Handler(void)
{
    asm("mov $0x14, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void Control_Protection_Handler(void)
{
    asm("mov $0x15, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC0_IRQHandler(void)
{
    asm("mov $0x0, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC1_IRQHandler(void)
{
    asm("mov $0x20, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC2_IRQHandler(void)
{
    asm("mov $0x21, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC3_IRQHandler(void)
{
    asm("mov $0x22, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC4_IRQHandler(void)
{
    asm("mov $0x23, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC5_IRQHandler(void)
{
    asm("mov $0x24, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC6_IRQHandler(void)
{
    asm("mov $0x25, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC7_IRQHandler(void)
{
    asm("mov $0x26, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC8_IRQHandler(void)
{
    asm("mov $0x27, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC9_IRQHandler(void)
{
    asm("mov $0x28, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC10_IRQHandler(void)
{
    asm("mov $0x29, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC11_IRQHandler(void)
{
    asm("mov $0x2A, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC12_IRQHandler(void)
{
    asm("mov $0x2B, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC13_IRQHandler(void)
{
    asm("mov $0x2C, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC14_IRQHandler(void)
{
    asm("mov $0x2D, %dl");
    asm("hlt");
}

__attribute__(( section(".interrupciones"))) void PIC15_IRQHandler(void)
{
    asm("mov $0x2E, %dl");
    asm("hlt");
}