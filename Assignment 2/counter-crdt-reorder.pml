//In this model, we have a network that can reorder messages
//Reordering is done using random receives, using ??
mtype = {INCREMENT, DECREMENT}

byte global_time = 0;
bool illegal = false;

chan r1_to_network = [5] of {mtype, byte};
chan r2_to_network = [5] of {mtype, byte};
chan r3_to_network = [5] of {mtype, byte};
chan network_to_r1 = [5] of {mtype, byte};
chan network_to_r2 = [5] of {mtype, byte};
chan network_to_r3 = [5] of {mtype, byte};

proctype Network() {
    mtype optype;
    byte op_ts;
    
    do
    :: r1_to_network?optype, op_ts ->
        atomic {
            network_to_r2!optype, op_ts;
            network_to_r3!optype, op_ts;
        }
    :: r2_to_network?optype, op_ts -> 
        atomic {
            network_to_r1!optype, op_ts;
            network_to_r3!optype, op_ts;
        }

    :: r3_to_network?optype, op_ts -> 
        atomic {
            network_to_r1!optype, op_ts;
            network_to_r2!optype, op_ts;
        }
    od;
}

proctype Replica(chan in, out) {
    int local_val = 0;
    mtype history[5];
    mtype optype;
    byte op_ts;
    
    do
    :: atomic {
        global_time < 5 ->

        if
        :: history[global_time] = INCREMENT; local_val++; optype = INCREMENT;
        :: history[global_time] = DECREMENT; local_val--; optype = DECREMENT;
        fi;

        op_ts = global_time;
        global_time++;

        //Broadcast

        out!optype, op_ts;
    }

    // Receive updates from other replicas, update history
    :: in??optype, op_ts -> 
        atomic {
            // Update, iff we have not updated for this timestamp before
            if
            :: history[op_ts] == 0 ->
                history[op_ts] = optype;
                if
                :: optype == INCREMENT -> local_val++;
                :: optype == DECREMENT -> local_val--;
                fi;
            fi;
        }
    od;
}

proctype Checker() {
    atomic
    {
        do
        :: (Replica[1]:history[0] == Replica[2]:history[0] &&
            Replica[1]:history[1] == Replica[2]:history[1] &&
            Replica[1]:history[2] == Replica[2]:history[2] &&
            Replica[1]:history[3] == Replica[2]:history[3] &&
            Replica[1]:history[4] == Replica[2]:history[4]) &&
           (Replica[1]:local_val != Replica[2]:local_val) -> illegal = true;

        :: (Replica[1]:history[0] == Replica[3]:history[0] &&
            Replica[1]:history[1] == Replica[3]:history[1] &&
            Replica[1]:history[2] == Replica[3]:history[2] &&
            Replica[1]:history[3] == Replica[3]:history[3] &&
            Replica[1]:history[4] == Replica[3]:history[4]) &&
           (Replica[1]:local_val != Replica[3]:local_val) -> illegal = true;

        :: (Replica[2]:history[0] == Replica[3]:history[0] &&
            Replica[2]:history[1] == Replica[3]:history[1] &&
            Replica[2]:history[2] == Replica[3]:history[2] &&
            Replica[2]:history[3] == Replica[3]:history[3] &&
            Replica[2]:history[4] == Replica[3]:history[4]) &&
           (Replica[2]:local_val != Replica[3]:local_val) -> illegal = true;
        od;
    }
}

//Initialization: start replicas with their channels
init {
    atomic {
        run Network();

        run Replica(network_to_r1, r1_to_network);
        run Replica(network_to_r2, r2_to_network);
        run Replica(network_to_r3, r3_to_network);
        
        run Checker();
    }
}

ltl sec {
    [] (!illegal) 
}