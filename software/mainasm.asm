#include "p16f1789.inc"
    
; CONFIG1
; __config 0xFFE4
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_ON & _CLKOUTEN_OFF & _IESO_ON & _FCMEN_ON
; CONFIG2
; __config 0xDAFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_HI & _LPBOR_OFF & _LVP_OFF
 
    ORG 0
    goto start
    
    ORG 4
    retfie
    
    start:
    banksel OSCCON
    movlw 0x7B ;internal oscyllator 16 MHz
    movwf OSCCON
    banksel TRISB
    clrf TRISB ; porty b jako wyjscia 
    banksel LATB
    movlw 0xFF

    loop:

    clrf  LATB
    clrf  LATB
    clrf  LATB
    clrf  LATB
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB
    
    clrf  LATB
    
    movwf LATB;17

    clrf  LATB
    clrf  LATB
    clrf  LATB
    clrf  LATB
    clrf  LATB

    goto loop

    end
