def tcl_write_print(conn,cmd): # write the return from a TCL command to stdout
    

    retval = conn.tcl(cmd)
    if type(retval) is str:
        print(retval)
    