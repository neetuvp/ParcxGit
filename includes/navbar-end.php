</ul>

<!-- Right navbar links -->
<ul class="navbar-nav ml-auto">
    <li class="nav-item">
    <select id="language">
            <option>English</option>
            <option>Arabic</option>
            <option>Spanish</option>
        </select>
    </li>
    <li class="nav-item">       
        <a href="<?=URL;?>index.php?logout='1'" class="btn btn-info logout-button">
            <i class="fas fa-sign-out-alt mr-1"></i>
            <span id="logout"><?=$json["logout"]?></span>
        </a>
    </li>
</ul>

</nav>
<!-- /.navbar -->