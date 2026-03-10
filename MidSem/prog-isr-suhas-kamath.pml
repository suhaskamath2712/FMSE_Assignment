int x = 0;
int y = 0;
bool isr_a_enabled = false;
bool isr_b_enabled = false;

proctype main()
{
    isr_a_enabled = true;
    x = -1;
    y = 2;
}

proctype isr_a()
{
    if
    ::  atomic {
            isr_a_enabled -> x = 1;
                        y = x + y;
                        isr_b_enabled = true;
                        assert (y < 5);
        }
    fi;
}

proctype isr_b()
{
    if
    :: atomic {isr_b_enabled && (x == 1) -> y = 5;}
    fi;
}

init
{
    run main();
    run isr_a();
    run isr_b();
}