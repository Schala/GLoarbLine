#!/linux/bin/perl

sub GetRecursiveDirs
{
    my %folders;
    foreach(@_)
    {
	my $dir = $_;
	next unless opendir HDIR, $dir;
	$folders{$dir} = 1;
	my @dirs;
	for(;;)
	{
	    my $fname = readdir HDIR;
	    next if $fname eq '.';
	    next if $fname eq '..';
	    last if $fname eq '';
	    next unless -d "$dir/$fname";
	    push @dirs, "$dir/$fname";
	}
	closedir HDIR;
    
        foreach(GetRecursiveDirs(@dirs))
	{
	   $folders{$_} = 1;
	}
	
    }
    return keys %folders;
}

sub GetFilesFromDirs
{
    my @files;
    foreach(@_)
    {
	my $dir = $_;
	next unless opendir HDIR, $dir;
	for(;;)
	{
	    my $fname = readdir HDIR;
	    next if $fname eq '.';
	    next if $fname eq '..';
	    last if $fname eq '';
	    next unless -f "$dir/$fname";
	    
	    push @files, "$dir/$fname" if ($fname =~ /\.cpp$/);
	    push @files, "$dir/$fname" if ($fname =~ /\.h$/);
	}
	closedir HDIR;
    }
    return sort @files;
}

sub CollectXSPMSG_fromFiles
{
    my $msgID=1;
    my %xspmsg;
    foreach(@_)
    {
	my $fname = $_;
	unless (open HFILE, $fname)
	{
	    print STDERR "Cannot open file $fname\n";
	    return;
	}
	my $content = join '', <HFILE>;
	close HFILE;
	
	my $foundChanges = 0;
	my $changed='';
	for(;;)
	{
	    if ($content =~ m/XSPMSG\(\s*(\d+)\s*,\s*("[^"]+")\s*\);/s)  #"
	    {
		$changed .= $`;
		$content = $';
		my $n = $1;
		my $msg = $2;
		
		if ($n == 0)
		{
		    $n = $msgID++;
		    $foundChanges = 1;
		}
		while (exists $xspmsg{$n} && ($xspmsg{$n} ne $msg))
		{
		    $n = ++$msgID;
		    $foundChanges = 1;
		}
		$xspmsg{$n} = $msg;
		$changed .= "XSPMSG($n,$msg);";
		
#		print "MSG $n= $msg\n";
	    }
	    else
	    {
		$changed .= $content;
		last;
	    }
	}
	if ($foundChanges)
	{
	    unless (open HFILE, '>'.$fname)
	    {
		print STDERR "Cannot write to file $fname\n";
		return;
	    }
	    print HFILE $changed;
	    close HFILE;
	    print "Changed $fname\n";
	}
    }
    if (exists $xspmsg{0})
    {
	print "MSG(0) was somehow created= ". $xspmsg{0}."\n";
	delete $xspmsg{0};
    }
    return \%xspmsg;
}

sub SaveMessages
{
    my $fname = shift;
    my $msgs = shift;

    unless (open HFILE, '>'.$fname)
    {
	print STDERR "Cannot write to file $fname\n";
	return;
    }
    my @ids;
    foreach( keys %$msgs)
    {
	my $id = $_;
	push @ids, sprintf('%8d',$id);
    }
    my $firstLine = 1;
    foreach(sort @ids)
    {
	my $n = $_;
	my $msg = $msgs->{0+$n};
	if ($firstLine)
	{
	    $firstLine = 0;
	    print HFILE ' ';
	}
	else
	{
	    print HFILE ',';
	}
	print HFILE "{$n, $msg}\n";
    }
    close HFILE;

    print "saved messages to $fname\n";
}

sub Job
{
    my @dirs = GetRecursiveDirs('../Sources');
    my @files = GetFilesFromDirs(@dirs);
    my $msgs = CollectXSPMSG_fromFiles(@files);
    SaveMessages('Messages.txt', $msgs);
}

&Job();
