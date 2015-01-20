
void serialization (void)
{
  bc_stmt stmt = NULL;
  char
    *var1 = "textvar",
    *value1 ="textvalue",
    *var2 = "intvar";
  int value2 = 444;

  bc_sql (&stmt, "INSERT INTO configs VALUES (?,?)");
  bc_binds (stmt, 1, var1);
  bc_binds (stmt, 2, value1);
  bc_step (stmt);
  bc_finalize (stmt);
  bc_sql (&stmt,"SELECT val FROM configs WHERE var=?");
  bc_binds (stmt, 1, var1);
  bc_step (stmt);
  assert(!strcmp (bc_gets (stmt, 0), value1));
  bc_finalize (stmt);

  /* test insertion and retrival of an integer: */

  bc_sql (&stmt, "INSERT INTO configs VALUES (?,?)");
  bc_binds (stmt, 1, var2);
  bc_bindi (stmt, 2, value2);
  bc_step (stmt);
  bc_finalize (stmt);
  bc_sql (&stmt,"SELECT val FROM configs WHERE var=?");
  bc_binds (stmt, 1, var2);
  bc_step (stmt);
  assert(bc_geti (stmt, 0) == value2);
  bc_finalize (stmt);

}
