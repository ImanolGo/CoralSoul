
while($true)
{
    # get Resolume process
    $resolume = Get-Process resolume -ErrorAction SilentlyContinue
    if (-Not ($resolume)) {
        Invoke-Item C:\Users\Public\CoralSoul\Resolume\*.avc
        Start-Sleep -Seconds 5
    }
    Remove-Variable resolume

    # get Ableton process
    $ableton = Get-Process ableton -ErrorAction SilentlyContinue
    if ($ableton) {
      Invoke-Item C:\Users\Public\CoralSoul\Ableton\*.als
      Start-Sleep -Seconds 5
    }
    Remove-Variable ableton

     # get CoralSoulApp process
     $unity = Get-Process CoralSoulUnity.exe -ErrorAction SilentlyContinue
     if ($unity) {
       Invoke-Item C:\Users\Public\CoralSoul\Unity\CoralSoulUnity.exe
       Start-Sleep -Seconds 5
     }
     Remove-Variable unity

    # get CoralSoulApp process
    $of = Get-Process CoralSoulApp.exe -ErrorAction SilentlyContinue
    if ($of) {
      Invoke-Item C:\Users\Public\CoralSoul\OF\CoralSoulApp.exe
      Start-Sleep -Seconds 5
    }
    Remove-Variable of


    Start-Sleep -Seconds 15
}






