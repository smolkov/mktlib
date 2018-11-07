// state_machine.h - state machine definition

/**
 * @defgroup statemachine
 * @ingroup market
 * @brief  Description
 *
 * @ingroup statemachine
 * @{
 * @file  market-statemachine.h
 * @brief header file description
 *
 * (c) 2008 - 2013 LAR Process Analysers AG - All rights reserved.
 *
 * @author C.Vogt
 *
 * $Id: $ $URL: $
 */

#ifndef STATE_MACHINE_H_INCLUDED
#define STATE_MACHINE_H_INCLUDED (1)

#ifndef true
#define true (1)
#endif

/**
 * For format and usage take a look at the state_machine_sample.c file
 *
 * State machine format is:
 *	"stateMachine"(initState, state_machine_description)
 *	state ...
 *	"stateMachineEnd"
 *
 * state format is:
 * "state"(number,name,	entryAction,exitAction)
 *		[timemode(go_state_number, timepar)]
 *		["goIf"(go_state_number, if_condition)
 *		|"ifDo"(if_condition, do_action)]
 *
 * number	- number of, a unique positive integer (some are reserved)
 * name		- a "string" enclosed in double quotes
 * action	- a C statement like "foo();bar();a=b", 
 *				use 0 if you do not want any action
 * timemode - may be omitted
 *			  or "goMs" with a timepar like 1000 with the number of milliseconds to wait.
 *			  or "goAt" with a timepar like "03:15:00" with the point in time to wait for.
 * mode		- may be omitted or "goMs" or "goAt"
 * condition- a C statement like "(error&&timeout())||31==control", 
 *				"true" can be used for last condition
 *
 * The execution of one step is done using:
 *	state_machine_step(control)
 * where
 *  control	- a known state control number or -1 for 'no command'
 **/


// state machine changed 2010-07-27
//
// state_description (new) 
//	provides the string name of the current state.
//
// void stateEnter(int state) allows actions when entering a new state.
// 	It is called after switching to the next state
//  and before calling the entry function of the next state
//  It is typically used to propagate state number and state_description.
// Refer to niti's control.c for an example.

// int stateChange(int state) allows overwriting of next state.
//  It is called after calling the exit function of the last state
//  and before setting the next state.
//  It is typically used to propagate state number.
/* Example usage:
int stateChange( int state )
{		
	//never tried: if (rebootFlagIsSet) state=0;

	// set internal state
	mkIset(control_subscription__internalStatus,state);

	return(state);
}
*/


extern char *state_machine_description;
extern char *state_description;
#define go(nextSt,st) do{ \
	state_machine(-1, 0, 1); \
	st=stateChange(nextSt); /* was stateChange before 2010-07-27 */\
	state_machine(-1, 1, 0); \
	}while(0)
#define stateMachine(st,desc) \
	char *state_machine_description=desc;\
	char *state_description="";  \
	int state_machine( int control, int entry, int exit) { int state; int error; \
	state = getState();\
	error = getError();\
	/*TEST printf("step starts with state=%d error=%d control=%d, entry=%d, exit=%d\n", state, error, control, entry, exit);\ */ \
	switch ( state ) { default:stateChange(0);
#define state_machine_step(c) state_machine(c, 0, 0);
#define stateMachineEnd ;break; } return(getError() );}
#define debug_out_state(s,str) do{debug_out(" ");debug_out(state_machine_description);debug_out(str);debug_out("state:");debug_out(s);debug_out("\n");}while(0)
#define state(no,s,e,x) ;break; case no : if(entry){state_description=s;stateEnter(no);debug_out_state(s," entry ");e;} else if(exit){debug_out_state(s," exit ");x;}
#define goMs(st,ms) ;if(entry){startTimer(ms);} else if(exit){stopTimer();} else if(timeout()){go(st,state);}
#define goAt(st) ;if(entry){startTimerAt();} else if(exit){stopTimerAt();} else if(timeoutAt()){go(st,state);}
#define goAt2(st) ;if(entry){startTimerAt2();} else if(exit){stopTimerAt2();} else if(timeoutAt2()){go(st,state);}
#define goDf(st) ;if(entry){startTimerDf();} else if(exit){stopTimerDf();} else if(timeoutDf()){go(st,state);}
#define goDf2(st) ;if(entry){startTimerDf2();} else if(exit){stopTimerDf2();} else if(timeoutDf2()){go(st,state);}
#define goIf(st,cond) else if ( cond ) {go(st,state);}
// FIX let state machine execute lines after ifDo :
//@TODO: ifDo() seemed buggy anyhow, when reentring to control state 2003 (clv2011-05-02)
#define ifDo(cond,prog) else if ( cond ) {prog;};if(entry){;} else if(exit){;}


#endif

