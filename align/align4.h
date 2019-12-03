#define ALIGNTO 4U
#define ALIGN4(len) ( ((len) + ALIGNTO - 1) & ~(ALIGNTO - 1))
