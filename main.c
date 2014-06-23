#include <stdio.h>
#include <stdlib.h>

#include "Simulation.h"

int main()
{
    initSimulation();
    
    int i;
    ExtY_Robot0_T *state;
    
    Robot0_U.v_ref = 1.0;
    for(i=0;i<100;i++)
    {
        state = Predict(1.0);
        
        printf("%lf %lf\n", state->x,state->y);
    }
    
    terminateSimulation();

    return 0;
}
