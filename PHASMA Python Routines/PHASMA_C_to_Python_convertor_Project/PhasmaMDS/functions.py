def tcl_write_print(conn,cmd):
    

    retval = conn.tcl(cmd)
    if type(retval) is str:
        print(retval)
    