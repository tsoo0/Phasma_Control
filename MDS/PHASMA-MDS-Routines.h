int 	OpenPHASMA_MDS(void);
int 	ClosePHASMA_MDS(void);
int 	Write_to_PHASMA_MDS(double t[], double x[], char tree_node_name[]);
int 	getMDSCurrentShot(void);
int 	IncrementMDSCurrentShot(void); 
int 	Write_to_PHASMA_2D_MDS(double image[], double x[], double y[], char tree_node_name[]);
int		CloseMDSTree(void);
void 	push_mds(void);