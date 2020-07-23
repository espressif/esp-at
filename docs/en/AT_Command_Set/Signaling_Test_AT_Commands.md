<a name="FACT-AT"></a>
## Signaling Test AT Commands 
- [AT+FACTPLCP](#cmd-FACTPLCP) -  Send with long or short PLCP(physical layer convergence procedure)  

 
<a name="cmd-FACTPLCP"></a>
### [AT+FACTPLCP](#FACT-AT)-Send with long or short PLCP(physical layer convergence procedure)  
Set Command:  

    AT+FACTPLCP=<enable>,<tx_with_long>
Response:

    OK

Parameters:

- **\<enable>** : Enable or disable manual configuration
  - 0 :  Disable manual configuration, it will use the defalut configuration    
  - 1 :  Enable manual configuration, send PLCP as `tx_with_long`    
- **\<tx\_with_long>** : Send with long PLCP or not
  - 0 : Send with short PLCP
  - 1 : Send with long PLCP
