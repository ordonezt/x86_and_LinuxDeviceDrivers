typedef struct{
    short int limite_0;
    int base_0  :24;
    int A       :1;
    union{
        int W   :1;
        int R   :1;
    };
    union{
        int ED  :1;
        int C   :1;
    };
    int bit_11  :1;
    int S       :1;
    int DPL     :2;
    int P       :1;
    short int limite_1:4;
    short int AVL:1;
    short int L:1;
    short int D_B:1;
    short int G:1;
    short int base_1:8;
}descriptor_segmento_t;

