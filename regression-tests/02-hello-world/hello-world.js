TIMEOUT(60000);

while(true) {
  log.log("> " + msg + "\n");
  if (msg.equals('Hello, world')) {
    log.testOK();
  }
  YIELD();
}
