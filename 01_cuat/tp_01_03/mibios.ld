ROM_ADDRESS = 0xFFFFF000;
RESET_ADDRESS = 0xFFFFFFF0;

SECTIONS
{
    . = ROM_ADDRESS;
    .rom_init : AT( ROM_ADDRESS ) {*(.romInit)}

    . = RESET_ADDRESS;
    .reset_vector : AT( RESET_ADDRESS ) {*(.resetVector)}
}