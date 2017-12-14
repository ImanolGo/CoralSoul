
while($true)
{
    # get Resolume process
    $resolume = Get-Process resolume -ErrorAction SilentlyContinue
    if (-Not ($resolume)) {
        Invoke-Item C:\Users\Public\Desktop\*.avc
        Start-Sleep -Seconds 5
    }
    Remove-Variable resolume

    # get Ableton process
    $ableton = Get-Process ableton -ErrorAction SilentlyContinue
    if ($ableton) {
      Invoke-Item C:\Users\Public\Desktop\*.als
      Start-Sleep -Seconds 5
    }
    Remove-Variable ableton

    # get CoralSoulApp process
    $of = Get-Process of -ErrorAction SilentlyContinue
    if ($of) {
      Invoke-Item C:\Users\Public\Desktop\CoralSoulApp\CoralSoulApp.exe
      Start-Sleep -Seconds 5
    }
    Remove-Variable of


    Start-Sleep -Seconds 15
}






