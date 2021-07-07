import qrcode
import base64


vachg_qngn = "REM0MjM6IFlPVSBUSElOSyBJVCBXT1VMRCBCRSBUSElTIEVBU1k/IQo=" 
snxr_qngn = "ZGlkIHlvdSB0cnkgdGhpcyBvbmUgdG9vCg=="
zber_qngn = "d2hhdGFib3V0dGhpcyxkaWR5b3V0cnl0aGlzdG9vLGxvbCxnaXZldXAK"
qr = qrcode.QRCode(
        version = 1,
        box_size = 10, 
        border = 5)


qr.add_data(base64.b64decode(vachg_qngn))
qr.make(fit=True)

img = qr.make_image(fill='black', back_color='white')
img.save('DC423_LIGHTSHOW.png')
test = 0
if(test == 1):
	abg_vzcbegnag = "ACTUAL MESSAGE GOES HERE "
