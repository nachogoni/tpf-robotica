import java.io.File;
import java.util.TimerTask;

public abstract class FileWatcher extends TimerTask {
  private File file;

  public FileWatcher( File file ) {
    this.file = file;
  }

  public final void run() {
	  while ( !file.exists() );
	
	long timeStamp = file.lastModified();
	long lastTimeStamp = timeStamp;
	
	while( true ){
		while( timeStamp == lastTimeStamp ){
			lastTimeStamp = file.lastModified();
			while ( lastTimeStamp == 0 )
				lastTimeStamp = file.lastModified();
		}

		onChange(file);
		timeStamp = lastTimeStamp;
	}
}

  protected abstract void onChange( File file );
}