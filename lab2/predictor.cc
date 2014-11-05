#include "predictor.h"

/////////////////////////////////////////////////////////////
// 2bitsat
/////////////////////////////////////////////////////////////

// using PC to index the table, instead of using global history
int two_bit_counter [4096];

void InitPredictor_2bitsat() {
	// 0 = Strong N, 3 = Strong T
	int i;
	for (i = 0; i < 4096; i++)
		two_bit_counter[i] = 1; // init to weakly not taken

}

bool GetPrediction_2bitsat(UINT32 PC) {
	int index = PC % 4096;
	return two_bit_counter[index] >= 2 ? TAKEN : NOT_TAKEN;
}

void UpdatePredictor_2bitsat(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int index = PC % 4096;
	if (resolveDir) {
		// Branch is T
		if (two_bit_counter[index] < 3) {
			two_bit_counter[index]++; // enforce taken state
		}
	}
	else {
		// Branch is F
		if (two_bit_counter[index] > 0) {
			two_bit_counter[index]--; // enforce not-taken state
		}
	}
}

/////////////////////////////////////////////////////////////
// 2level
/////////////////////////////////////////////////////////////

// history table is 6'b wide => 64 entries per predictor table
unsigned int private_hist_table [512];
unsigned int private_pred_table [8][64]; // 8 predictor tables, 64 entries

void InitPredictor_2level() {
	// 0 = Strong N, 3 = Strong T
	int i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 64; j++)
			private_pred_table[i][j] = 1; // init to weakly not taken
}

bool GetPrediction_2level(UINT32 PC) {
	int hist_table_index = (PC >> 3) & 0b111111111; // 9 bit (mod 512)
	int pred_table_selector = 0b111 & PC; //3 bit (mod 8). this chooses which prediction table to read prediction
	int pred_table_index = private_hist_table[hist_table_index]; // this is also the history, used to index the prediction table

	return private_pred_table[pred_table_selector][pred_table_index] >= 2 ? TAKEN : NOT_TAKEN;
}

void UpdatePredictor_2level(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int hist_table_index = (PC >> 3) & 0b111111111; // 9 bit (mod 512)
	int pred_table_selector = 0b111 & PC; //3 bit (mod 8)
        int pred_table_index = private_hist_table[hist_table_index]; // this is also the history

	if (resolveDir){
		// Branch is T
		if (private_pred_table[pred_table_selector][pred_table_index] < 3) {
			private_pred_table[pred_table_selector][pred_table_index]++; // enforce taken state
		}

	} else {
		// Branch is F
		if (private_pred_table[pred_table_selector][pred_table_index] > 0) {
			private_pred_table[pred_table_selector][pred_table_index]--; // enforce not-taken state
		}
	}


	pred_table_index = pred_table_index << 1; // shift history bits
	if (resolveDir){
		// Branch is T
		pred_table_index += 1; // update latest result to history
	}
	private_hist_table[hist_table_index] = pred_table_index & 0b111111; //equivalent to mod 64, makes sure history is 6 bits
}

/////////////////////////////////////////////////////////////
// openend
/////////////////////////////////////////////////////////////

//Gshare
unsigned int gshare_pred_table [4096];// gshare is table of 2-bit saturating counters, indexed by global history
unsigned int glob_hist;

void InitPredictor_openend_gshare() {
	// 0 = Strong N, 3 = Strong T
	int i;
	for (i = 0; i < 4096; i++)
		gshare_pred_table[i] = 2; // init to weakly taken
}

bool GetPrediction_openend_gshare(UINT32 PC) {
	int index = 0b111111111111 & (glob_hist ^ PC); // index is XOR with PC, to account for history independent branches
	// branches could yield different results (even on the same PC) with different global history.
	return gshare_pred_table[index] >= 2 ? TAKEN : NOT_TAKEN;
}

void UpdatePredictor_openend_gshare(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int index = 0b111111111111 & (glob_hist ^ PC); // XOR the PC, trait of Gshare, explained above

	if (resolveDir){
		// Branch is T
		if (gshare_pred_table[index] < 3) {
			gshare_pred_table[index]++; // enforce taken state
		}

	} else {
		// Branch is F
		if (gshare_pred_table[index] > 0) {
			gshare_pred_table[index]--; // enforce not-taken state
		}
	}
}


// 2-level, using 3-bit saturating counters
unsigned int openend_private_hist_table [2048]; // 2048 entries of history, indexed by PC
unsigned int openend_private_pred_table [8][2048]; // 8 predictor tables, 2048 entries

void InitPredictor_openend_2level() {
	// 0 = Strong N, 5 = Strong T
	int i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 2048; j++)
			openend_private_pred_table[i][j] = 3; // init to weakly taken
}

bool GetPrediction_openend_2level(UINT32 PC) {
	int hist_table_index = (PC >> 3) & 0b11111111111; // 11 bit, mod 2048
	int pred_table_selector = 0b111 & PC; //3 bit
	int pred_table_index = openend_private_hist_table[hist_table_index]; // this is also the history as well as index to pred. table

	return openend_private_pred_table[pred_table_selector][pred_table_index] >= 3 ? TAKEN : NOT_TAKEN;
}

void UpdatePredictor_openend_2level(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int hist_table_index = (PC >> 3) & 0b11111111111; // 11 bit, mod 2048
	int pred_table_selector = 0b111 & PC; //3 bit
        int pred_table_index = openend_private_hist_table[hist_table_index]; // this is also the history as well as index to pred. table

	if (resolveDir){
		// Branch is T
		if (openend_private_pred_table[pred_table_selector][pred_table_index] < 5) {
			openend_private_pred_table[pred_table_selector][pred_table_index]++; // enforce taken state
		}

	} else {
		// Branch is F
		if (openend_private_pred_table[pred_table_selector][pred_table_index] > 0) {
			openend_private_pred_table[pred_table_selector][pred_table_index]--; // enforce not-taken state
		}
	}

	// update private/local history
	pred_table_index = pred_table_index << 1;
	if (resolveDir){
		// Branch is T
		pred_table_index += 1;
	}
	openend_private_hist_table[hist_table_index] = pred_table_index & 0b11111111111; //equivalent to mod 2048, ensure history bits stay 11 bits
}


unsigned int chooser_counter [4096];// chooser is table of 2-bit saturating counters, indexed by global history

void InitPredictor_openend() {
	InitPredictor_openend_2level();
	InitPredictor_openend_gshare();

	// 0 = Strong pred 1, 3 = Strong pred 2
	int i;
	for (i = 0; i < 4096; i++)
		chooser_counter[i] = 3; // init to strongly using global history predictor 
}

bool GetPrediction_openend(UINT32 PC) {
	int index = glob_hist;
	return chooser_counter[index] >= 2? GetPrediction_openend_gshare(PC) : GetPrediction_openend_2level(PC);
}

void UpdatePredictor_openend(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget) {
	int index = glob_hist;

	if (GetPrediction_openend_gshare(PC) == GetPrediction_openend_2level(PC)) { // if both predictors have the same result, don't affect chooser
		// do nothing
	}else if ((resolveDir && predDir) || (!resolveDir && !predDir)) {// if prediction was correct, lean towards the used predictor
		// predictor used was correct
		
		if (chooser_counter[index] >= 2) { // gshare was used
			if (chooser_counter[index] < 3)
				chooser_counter[index]++; // enforce gshare
		} else { // 2-level local history was used
			if (chooser_counter[index] > 0)
				chooser_counter[index]--; // enforce 2-level local history	
		}
	}
	else {
		// predictor used was wrong, lean towards the unused predictor 
		if (chooser_counter[index] >= 2) { // gshare was used
			chooser_counter[index]--; // enforce 2-level local history
		} else { // 2-level local history was used
			chooser_counter[index]++; // enforce gshare	
		}
	}

	// update predictor results
	UpdatePredictor_openend_2level(PC, resolveDir,  predDir,  branchTarget);
	UpdatePredictor_openend_gshare(PC, resolveDir,  predDir,  branchTarget);

	// update global history registor (used by chooser and gshare)
	glob_hist = glob_hist << 1;
	if (resolveDir){
		// Branch is T
		glob_hist += 1;
	}
	glob_hist = glob_hist & 0b111111111111; //mod 4096, ensure history bit stay 12 bits

}

