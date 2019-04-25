# FileTransfer-TLS-Socket
File transfer app  using C sockets and OpenSSL that transfers files across entities. 
Put all certificate file paths, port number and buffer size inside test_app.cpp

### Build
- <code> make app //if app binary is not present </code>
- <code> make clean app //is app binary is present </code>

### Run 
#### On the sending side: 
<code> sudo ./app send IP_ADDR_RECV FILE_PATH </code>

#### On the receiving side
<code> sudo ./app receive FILE_LOCATION </code>

### Demo 
Sending side ![Sending side](/send.png) 
Receiving side ![Receiving side](/receive.png) 
