echo "Run All Script "
echo "Starting ... "


while($true)
{
    # get Resolume process
    $resolume = Get-Process Arena -ErrorAction SilentlyContinue
    if (-Not ($resolume)) {
        echo "Process Arena Not Found"
        echo "Starting Resolume Arena ..."
        Invoke-Item C:\Users\user\CoralSoul\Resolume\*.avc
        Start-Sleep -Seconds 5
    }
    Remove-Variable resolume

    # get Ableton process
    $ableton = Get-Process ableton -ErrorAction SilentlyContinue
    if (-Not ($ableton)) {
      echo "Process Ableton Not Found"
      echo "Starting Ableton ..."
      Invoke-Item C:\Users\user\CoralSoul\Ableton\*.als
      Start-Sleep -Seconds 5
    }
    Remove-Variable ableton

     # get CoralSoulInteractive process
     $interactive = Get-Process Interactive_Waterfall_020218Interactive -ErrorAction SilentlyContinue
     if (-Not ($interactive)) {
      echo "Process Interactive Not Found ..."
      echo "Starting Interactive ..."
       Invoke-Item C:\Users\user\CoralSoul\Unity\Interactive\Interactive_Waterfall_020218Interactive.exe
       Start-Sleep -Seconds 5
     }
     Remove-Variable interactive

    # get CoralSoulMaster process
    $master = Get-Process StateMachine -ErrorAction SilentlyContinue
    if (-Not ($master)) {
      echo "Process State Machine Not Found ..."
      echo "Starting State Machine"
      Invoke-Item C:\Users\user\CoralSoul\Unity\StateMachine\StateMachine.exe
      Start-Sleep -Seconds 5
    }
    Remove-Variable master
  

    # get CoralSoulApp process
    $of = Get-Process CoralSoulApp -ErrorAction SilentlyContinue
    if (-Not ($of)) {
      echo "Process OF Not Found"
      echo "Starting OF ..."
      Invoke-Item C:\Users\user\CoralSoul\OF\CoralSoulApp.exe
      Start-Sleep -Seconds 5
    }
    else{

    }
    Remove-Variable of


    Start-Sleep -Seconds 15
}






